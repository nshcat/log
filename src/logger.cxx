#include <stdexcept>
#include <mutex>


#include "console_target.hxx"
#include "logger.hxx"
#include "log_target.hxx"

namespace lg
{
	logger& logger::instance()
	{
		static logger instance{};
		return instance;
	}

	// Adds ConsoleTarget as default target
	void logger::default_init(severity_level p_lvl)
	{
		static console_target<> target{ p_lvl };

		instance()._add_target(&target);
	}

	void logger::_add_target(ut::observer_ptr<log_target> target)
	{
		if(target)
			m_Targets.push_back(target);
		else throw std::runtime_error("logger::add_target: target==nullptr!");
	}

	void logger::add_target(ut::observer_ptr<log_target> target)
	{
		instance()._add_target(target);
	}

	void logger::operator+= (const log_entry& entry)
	{
		std::lock_guard<std::recursive_mutex> lock(m_Mtx);

		for (auto& target : m_Targets)
		{
			// Log only if verbosity level does not exclude entry level
			if (target->level() >= entry.level())
			{
				target->write(entry);
			}
		}
	}

	void logger::lock()
	{
		m_Mtx.lock();
	}

	void logger::unlock()
	{
		m_Mtx.unlock();
	}
}