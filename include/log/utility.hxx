#pragma once
#include <type_traits>


namespace lg
{
	namespace internal
	{
		// Cast scoped enumeration to underlying type
		template< typename Tenum >
		constexpr auto enum_to_ult(Tenum e) -> typename std::underlying_type<Tenum>::type
		{
			return static_cast<typename std::underlying_type<Tenum>::type>(e);
		}

		// Cast underlying type to scoped enumeration
		template< typename Tenum >
		constexpr auto ult_to_enum(typename std::underlying_type<Tenum>::type var) -> Tenum
		{
			return static_cast<Tenum>(var);
		}
	}
}