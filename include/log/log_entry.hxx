#pragma once

#include <string>
#include <sstream>
#include <ctime>
#include <exception>
#include <ut/console_color.hxx>

#include "severity_level.hxx"
//#include "FormatManip.hxx"

namespace lg
{
	class log_entry
	{
		public:
			log_entry(std::string file, size_t line);
			log_entry(std::string file, size_t line, bool is_bare);

		public:
			log_entry& operator<< (severity_level lvl);
			log_entry& operator<< (ut::console_color clr);
			log_entry& operator<< (const std::string& str);
			//log_entry& operator<< (const FormatManipulator& manip);
			log_entry& operator<< (const std::exception& ex);

			template< typename T >
			log_entry& operator<< (T&& t)
			{
				m_Message << ::std::forward<T>(t);
				return *this;
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