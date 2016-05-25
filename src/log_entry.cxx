#include <utility>
#include <ctime>
#include <iomanip>
#include <vector>
#include <chrono>

#include "log_entry.hxx"
#include "utility.hxx"


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

	log_entry& log_entry::operator<< (const std::exception& ex)
	{
		m_Message << ex.what();
		return *this;
	}

	log_entry& log_entry::operator<< (severity_level lvl)
	{
		m_Level = lvl;
		return *this;
	}

	log_entry& log_entry::operator<< (ut::console_color clr)
	{
		m_Color = clr;
		return *this;
	}

	log_entry& log_entry::operator<< (const std::string& str)
	{
		m_Message << str;
		return *this;
	}

	// TODO fix this
	/*log_entry& log_entry::operator<< (const FormatManipulator& manip)
	{
		m_Message << manip.str();
		return *this;
	}*/

	std::string log_entry::message() const
	{
		return m_Message.str();
	}

	std::string log_entry::file() const
	{
		return m_File;
	}

	std::string log_entry::time_string() const
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
		static const char* names[6] =
		{
			"Fatal",
			"Error",
			"Warning",
			"Info",
			"Debug"
		};

		// Get underlying type and return string
		auto x = internal::enum_to_ult(m_Level);
		
		// Check for correctness
		if(x < sizeof(names))
			return names[x];
		else throw ::std::runtime_error("Invalid severity level for log entry! (Couldn't convert level to string)");
	}
}