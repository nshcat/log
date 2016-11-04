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
#include <ut/console_color.hxx>

#include "default_formatter.hxx"
#include "log_entry.hxx"

namespace lg
{
	void default_formatter::operator()(std::ostream& str, const log_entry& entry)
	{
		if (entry.bare())
		{
			str << std::setw(20)
				<< "| ";
		}
		else
		{
			str << "["
				<< std::setw(8)
				<< std::right
				<< entry.time_string()
				<< "] "
				<< ut::foreground(entry.color())
				<< std::setw(7)
				<< std::right
				<< entry.level_string() << ut::reset_color
				<< "| ";
		}
		str	<< entry.message()
			<< "\n";
			
		str.flush();
	}
}