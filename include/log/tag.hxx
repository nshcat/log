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

namespace lg
{
	namespace internal
	{
		// Structure used to hold tag label and signal the implementation
		// that its meant to be interpreted as such
		class tag_t
		{
			public:
				// Taking the string by value is by design here, since this way
				// we can take advantage of move semantics
				tag_t(::std::string p_str)
					: m_Tag{::std::move(p_str)}
				{
					
				}
				
				tag_t() = default;
				tag_t(const tag_t&) = default;
				tag_t(tag_t&&) = default;
				
				tag_t& operator=(const tag_t&) = default;
				tag_t& operator=(tag_t&&) = default;
				
			public:
				auto value() const &
					-> const ::std::string&
				{
					return m_Tag;
				}
					
				auto value() &&
					-> ::std::string&&
				{
					return ::std::move(m_Tag);
				}
		
			private:
				::std::string m_Tag{ };
		};
	}
	
	// Passing string by value is intented here in order to leverage move semantics
	auto tag(::std::string) -> internal::tag_t;
}
