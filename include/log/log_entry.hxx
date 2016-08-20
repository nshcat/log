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
#include <sstream>
#include <ctime>
#include <exception>
#include <ut/console_color.hxx>

#include "severity_level.hxx"

namespace lg
{
	class log_entry
	{
		public:
			log_entry(const log_entry&) = delete;
			log_entry(log_entry&&) = default;
			
			log_entry& operator=(const log_entry&) = delete;
			log_entry& operator=(log_entry&&) = default;
		
		public:
			log_entry(std::string file, size_t line);
			log_entry(std::string file, size_t line, bool is_bare);
			
		public:
			log_entry operator<< (severity_level lvl) &&;
			log_entry operator<< (ut::console_color clr) &&;
			log_entry operator<< (const std::exception& ex) &&;

			template< typename T >
			log_entry operator<< (T&& t) &&
			{
				m_Message << ::std::forward<T>(t);
				return std::move(*this);
			}

		public:
			bool bare() const;
			const std::time_t& time() const;
			std::string message() const;
			std::string file() const;
			std::string time_string() const;
			size_t line() const;
			severity_level level() const;
			std::string level_string() const;
			ut::console_color color() const;

		private:
			std::stringstream	m_Message;
			std::string			m_Timestamp;
			std::string			m_File;
			bool				m_IsBare;
			size_t				m_Line;
			severity_level		m_Level;
			ut::console_color	m_Color;
			std::time_t			m_Time;
	};
}