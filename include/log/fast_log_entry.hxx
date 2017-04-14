#pragma once

#include <array>
#include <cstdint>
#include <ctime>
#include <initializer_list>
#include <type_traits>
#include <ut/string_view.hxx>
#include <ut/type_traits.hxx>
#include <ut/cast.hxx>
#include <ut/utility.hxx>

#include "severity_level.hxx"

namespace lg
{
	namespace internal
	{
		// TODO move all this internal stuff into own header file to clean this file up

		enum class entry_type
			: uint8_t
		{
			none = 0x0,
			u8,
			i8,
			u16,
			i16,
			u32,
			i32,
			u64,
			i64,
			ptr,
			char_ptr,
			boolean,
			floating32,
			floating64
		};

		template< typename T >
		struct is_valid_type
			: ut::contains<
			  		T,
					uint8_t, int8_t,
					uint16_t, int16_t,
					uint32_t, int32_t,
					uint64_t, int64_t,
					void*, char*,
					bool,
					float, double
			  >
		{
		};

		template< typename T >
		constexpr bool is_valid_type_v = is_valid_type<T>::value;


		template< ::std::size_t N, typename... Ts >
		constexpr bool fits_impl()
		{
			auto x = { sizeof(Ts)... };

			::std::size_t sz = sizeof...(Ts);

			for(const auto& y: x)
				sz += y;

			return sz <= N;
		}

		template< ::std::size_t N, typename... Ts >
		struct fits
			: ::std::integral_constant<bool, fits_impl<N, Ts...>()>
		{
		};

		template< ::std::size_t N, typename... Ts >
		constexpr bool fits_v = fits<N, Ts...>::value;

		
		template< typename T >
		struct convert_pointer
		{
			using type = T;
		};

		template< >
		struct convert_pointer <char*>
		{
			using type = char*;
		};

		template< typename T >
		struct convert_pointer <T*>
		{
			using type = void*;
		};

		template< typename T >
		using convert_pointer_t = typename convert_pointer<T>::type;




		template< typename T >
		constexpr entry_type get_entry_type();

		template< >
		constexpr entry_type get_entry_type<uint8_t>()
		{
			return entry_type::u8;
		}

		template< >
		constexpr entry_type get_entry_type<int8_t>()
		{
			return entry_type::i8;
		}

		template< >
		constexpr entry_type get_entry_type<uint16_t>()
		{
			return entry_type::u16;
		}

		template< >
		constexpr entry_type get_entry_type<int16_t>()
		{
			return entry_type::i16;
		}

		template< >
		constexpr entry_type get_entry_type<uint32_t>()
		{
			return entry_type::u32;
		}

		template< >
		constexpr entry_type get_entry_type<int32_t>()
		{
			return entry_type::i32;
		}

		template< >
		constexpr entry_type get_entry_type<uint64_t>()
		{
			return entry_type::u64;
		}

		template< >
		constexpr entry_type get_entry_type<int64_t>()
		{
			return entry_type::i64;
		}

		template< >
		constexpr entry_type get_entry_type<void*>()
		{
			return entry_type::ptr;
		}

		template< >
		constexpr entry_type get_entry_type<char*>()
		{
			return entry_type::char_ptr;
		}

		template< >
		constexpr entry_type get_entry_type<bool>()
		{
			return entry_type::boolean;
		}

		template< >
		constexpr entry_type get_entry_type<float>()
		{
			return entry_type::floating32;
		}

		template< >
		constexpr entry_type get_entry_type<double>()
		{
			return entry_type::floating64;
		}

		template< ::std::size_t N >
		class fast_log_entry
		{
			using data_type = ::std::array<char, N>;

			public:
				constexpr static ::std::size_t data_length = N;

			public:
				template< typename... Ts >
				fast_log_entry(	ut::string_view p_fmt,
								severity_level p_lvl,
								const ::std::time_t& p_time,
								Ts&&... p_args )
					: m_FmtStr{p_fmt}, m_Time{p_time}, m_Level{p_lvl}
				{
					// Type and total size checking is done in serialize.
					serialize(::std::forward<Ts>(p_args)...);
				}

				~fast_log_entry() = default;

				fast_log_entry(const fast_log_entry&) = default;
				fast_log_entry& operator=(const fast_log_entry&) = default;

				// Disallow moving objects of this type, since none of the members
				// can be moved easily.
				fast_log_entry(fast_log_entry&&) = delete;
				fast_log_entry& operator=(fast_log_entry&&) = delete;

			public:
				// IMPORTANT: all pointers besides char* need to be converted to void*
				template< typename... Ts >
				auto serialize(Ts&&... p_args)
					-> void;

				template< typename F >
				auto deserialize(F&& p_func)
					-> void;

			public:
				auto format_string() const
					-> ut::string_view;

				auto timestamp() const
					-> const ::std::time_t&;

				auto level() const
					-> severity_level;

			private:
				template< typename T, typename It >
				auto serialize_impl(It& p_begin, T p_val)
					-> void;
										
			private:
				ut::string_view m_FmtStr;	//< View to the format string.
				::std::time_t m_Time;		//< Timestamp
				severity_level m_Level; 	//< Severity level
				data_type m_Data;			//< Byte array containing serialized format arguments
				
		};


		template< ::std::size_t N >
		template< typename... Ts >
		auto fast_log_entry<N>::serialize(Ts&&... p_args)
			-> void
		{
			static_assert( ut::conjunction<is_valid_type<convert_pointer_t<::std::decay_t<Ts>>>...>::value,
					"fast_log_entry: Invalid type supplied. Only fundamental types are allowed." );

			static_assert( fits<N, ::std::decay_t<Ts>...>::value,
					"fast_log_entry: Supplied values exceed storage capacity!" );
		
			auto t_iter = m_Data.begin();

			auto x = { (serialize_impl<convert_pointer_t<::std::decay_t<Ts>>>(t_iter, p_args), 0)... };
			(void)x;
		}

		template< ::std::size_t N >
		template< typename T, typename It >
		auto fast_log_entry<N>::serialize_impl(It& p_begin, T p_val)
			-> void
		{
			// Determine entry type
			constexpr auto t_type = get_entry_type<T>();

			// Write it out
			p_begin = ut::to_bytes(p_begin, (uint8_t)ut::enum_cast(t_type));
			p_begin = ut::to_bytes(p_begin, p_val);
		}

		template< ::std::size_t N >
		template< typename F >
		auto fast_log_entry<N>::deserialize(F&& p_func)
			-> void
		{
			// Current position in data block
			auto t_iter =  m_Data.begin();
			// Current entry type
			entry_type t_type = entry_type::none;

			while(t_iter != m_Data.end())
			{
				// Get next entry type
				t_type = ut::enum_cast<entry_type>(ut::from_bytes<uint8_t>(t_iter, m_Data.end()));

				// Only continue if there are entries left
				if(t_type == entry_type::none)
					break;

				// Switch entry type. TODO do less ugly.
				switch(t_type)
				{
					case entry_type::u8:
						p_func(ut::from_bytes<uint8_t>(t_iter, m_Data.end()));
						break;

					case entry_type::i8:
						p_func(ut::from_bytes<int8_t>(t_iter, m_Data.end()));
						break;

					case entry_type::u16:
						p_func(ut::from_bytes<uint16_t>(t_iter, m_Data.end()));
						break;

					case entry_type::i16:
						p_func(ut::from_bytes<int16_t>(t_iter, m_Data.end()));
						break;

					case entry_type::u32:
						p_func(ut::from_bytes<uint32_t>(t_iter, m_Data.end()));
						break;

					case entry_type::i32:
						p_func(ut::from_bytes<int32_t>(t_iter, m_Data.end()));
						break;

					case entry_type::u64:
						p_func(ut::from_bytes<uint64_t>(t_iter, m_Data.end()));
						break;

					case entry_type::i64:
						p_func(ut::from_bytes<int64_t>(t_iter, m_Data.end()));
						break;

					case entry_type::ptr:
						p_func(ut::from_bytes<void*>(t_iter, m_Data.end()));
						break;

					case entry_type::char_ptr:
						p_func(ut::from_bytes<char*>(t_iter, m_Data.end()));
						break;

					case entry_type::boolean:
						p_func(ut::from_bytes<bool>(t_iter, m_Data.end()));
						break;

					case entry_type::floating32:
						p_func(ut::from_bytes<float>(t_iter, m_Data.end()));
						break;

					case entry_type::floating64:
						p_func(ut::from_bytes<double>(t_iter, m_Data.end()));
						break;

					default:
						// This can't happen.
						break;
				}
			}
		}
	}
}
