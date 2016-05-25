#pragma once

#include <string>
#include <ostream>

namespace lg
{
	class log_entry;

	class default_formatter
	{
		public:
			void operator()(std::ostream&, const log_entry&);
	};
}
