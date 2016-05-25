#pragma once

#include "default_formatter.hxx"
#include "log_target.hxx"
#include "log_entry.hxx"

namespace lg
{
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