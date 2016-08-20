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

#include <fstream>

#include "default_formatter.hxx"
#include "log_target.hxx"
#include "log_entry.hxx"

namespace lg
{
	// Log target that writes to a file, either truncating or appending to it.
	template< typename Tformat = default_formatter >
	class file_target
		: public log_target
	{
		static_assert(internal::is_formatter<Tformat>::value, "Tformat is not a valid log formatter!");

		public:
			// Construct new file target with given severity threshold and file path.
			file_target(severity_level p_lvl, const std::string& p_path, bool p_append)
				: 	log_target(p_lvl),
					m_File(
						p_path,
						std::ios_base::out | (p_append ? std::ios_base::app : std::ios_base::trunc)
					)
			{
				
			}
			
			file_target(severity_level p_lvl, const std::string& p_path)
				: file_target(p_lvl, p_path, true)
			{
				
			}

		public:
			// Write to standard out.
			// TODO maybe write errors to cerr instead. This could be checked here.
			virtual void write(const log_entry& entry) override
			{
				// Format and write entry to console output stream
				m_Formatter(m_File, entry);
			}

		private:
			Tformat m_Formatter;	// Formatter used by this target
			std::ofstream m_File;	// File output stream
	};
}