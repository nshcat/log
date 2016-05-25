#include <ut/console_color.hxx>
#include <iomanip>
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
	}
}