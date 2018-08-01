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

#include <string>
#include <ostream>

namespace lg
{
	class log_entry;
	
	// A formatter that mimics the diagnostics style of clang.
	class clang_formatter
	{
		public:
			clang_formatter() = default;
			
			clang_formatter(bool p_indent)
				: m_Indent{p_indent}
			{
			}
	
		public:
			void operator()(std::ostream&, const log_entry&);
			
		protected:
			auto update_width(const log_entry&)
				-> void;
			
		protected:
			bool m_Indent{true};
			::std::size_t m_LastHeaderWidth{ }; //< The width of the header (tag and severity string) of the last non-bare message
	};
}
