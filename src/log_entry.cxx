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

#include <utility>
#include <array>
#include <ctime>
#include <iomanip>
#include <vector>
#include <chrono>
#include <ut/cast.hxx>
#include <ut/string_view.hxx>

#include "log_entry.hxx"

namespace lg
{
	log_entry::log_entry(std::string file, size_t line)
		:	log_entry(std::move(file), line, false)
	{

	}

	log_entry::log_entry(std::string file, size_t line, bool is_bare)
		:	m_File{ std::move(file) }, 
			m_Line{ line },
			m_IsBare{ is_bare },
			m_Color{ ut::console_color::reset },
			m_Level{ severity_level::info },
			m_Time{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) }
	{
		// Get timestamp
		std::tm* tm = std::localtime(&m_Time);

		// Format timestamp
		std::ostringstream ss;
		ss << std::put_time(tm, /*"%T"*/"%H:%M:%S"); // TODO "hack".. libstdc++ is still not compatible with c++11, it doesnt support %T

		m_Timestamp = ss.str();
	}

	log_entry log_entry::operator<< (const std::exception& ex) &&
	{
		m_Message << ex.what();
		return std::move(*this);
	}

	log_entry log_entry::operator<< (severity_level lvl) &&
	{
		m_Level = lvl;
		return std::move(*this);
	}

	log_entry log_entry::operator<< (ut::console_color clr) &&
	{
		m_Color = clr;
		return std::move(*this);
	}
	
	log_entry log_entry::operator<< (internal::tag_t tag) &&
	{
		m_Tag = tag.value();
		return std::move(*this);
	}

	std::string log_entry::message() const
	{
		return m_Message.str();
	}
	
	const std::string& log_entry::tag() const
	{
		return m_Tag;
	}

	const std::string& log_entry::file() const
	{
		return m_File;
	}

	const std::string& log_entry::time_string() const
	{
		return m_Timestamp;
	}

	const std::time_t& log_entry::time() const
	{
		return m_Time;
	}

	size_t log_entry::line() const
	{
		return m_Line;
	}

	severity_level log_entry::level() const
	{
		return m_Level;
	}

	bool log_entry::bare() const
	{
		return m_IsBare;
	}

	ut::console_color log_entry::color() const
	{
		return m_Color;
	}

	std::string log_entry::level_string() const
	{
		// string_view would be better here, but ut::string_view is missing constexpr
		// support for the const char* constructor
		static constexpr ::std::array<const char*, 6> names =
		{
			"Fatal",
			"Error",
			"Warning",
			"Info",
			"Debug"
		};

		// Get underlying type and return string
		std::size_t x = ut::enum_cast(m_Level);
		
		// Check for correctness
		if(x < sizeof(names))
			return names[x];
		else throw ::std::runtime_error("Invalid severity level for log entry! (Couldn't convert level to string)");
	}
}
