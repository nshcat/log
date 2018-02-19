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

#include <iomanip>
#include <algorithm>
#include <cctype>
#include <ut/console_color.hxx>

#include "clang_formatter.hxx"
#include "log_entry.hxx"

namespace lg
{
	void clang_formatter::operator()(std::ostream& str, const log_entry& entry)
	{
		if (entry.bare())
		{
			str << std::setw(12)
				<< " ";
		}
		else
		{
			// Retrieve level string and transform to lower case only
			auto t_lvl = entry.level_string();
			
			std::transform(t_lvl.begin(), t_lvl.end(), t_lvl.begin(),
				[](char c) -> char
				{
					return static_cast<char>(std::tolower(c));
				}
			);
			
			// Output tag if available
			if(!entry.tag().empty())
			{
				str << entry.tag() << ": ";
			}
			
			// Output message level
			str << ut::foreground(entry.color())
				<< t_lvl
				<< ut::reset_color
				<< ": ";
		}
		str	<< entry.message()
			<< "\n";
			
		str.flush();
	}
}
