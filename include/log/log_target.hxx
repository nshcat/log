#pragma once

#include <iostream>
#include <type_traits>
#include <ut/type_traits.hxx>

#include "severity_level.hxx"

namespace lg
{
	class log_entry;

	class log_target
	{
		public:
			log_target(severity_level p_lvl)
				: m_Threshold(p_lvl)
			{

			}

		public:
			virtual ~log_target(){}

		public:
			virtual void write(const log_entry&) = 0;

		public:
			severity_level level() const
			{
				return m_Threshold;
			}

		private:
			severity_level   m_Threshold;
	};

	namespace internal
	{
		template< typename T >
		using is_formatter_test =
			decltype( ::std::declval<T>()( ::std::declval<::std::ostream&>(), ::std::declval<log_entry&>() ) );
		
		template< typename T >
		using is_formatter = ::ut::is_detected<is_formatter_test, T>;
	}
}