/*
	Copyright (c) 2016 nshcat

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute,
	sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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