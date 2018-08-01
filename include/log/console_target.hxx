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

#include "default_formatter.hxx"
#include "log_target.hxx"
#include "log_entry.hxx"

namespace lg
{
	// Log target that writes to the standard output stream.
	template< typename Tformat = default_formatter >
	class console_target
		: public log_target
	{
		static_assert(internal::is_formatter<Tformat>::value, "Tformat is not a valid log formatter!");

		public:
			// Construct new console target with given severity threshold.
			console_target(severity_level p_lvl)
				: log_target(p_lvl)
			{

			}
			
			// Construct new console target with given severity threshold and
			// formatter instance
			console_target(severity_level p_lvl, const Tformat& p_fmt)
				: log_target(p_lvl), m_Formatter{p_fmt}
			{
			}

		public:
			// Write to standard out.
			// TODO maybe write errors to cerr instead. This could be checked here.
			virtual void write(const log_entry& entry) override
			{
				// Format and write entry to console output stream
				m_Formatter(std::cout, entry);
			}

		private:
			Tformat m_Formatter;
	};
}
