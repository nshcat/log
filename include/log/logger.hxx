#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <ut/observer_ptr.hxx>

#include "log_entry.hxx"

namespace lg
{
	class log_target;

	class logger
	{
		private:
			logger()
			{
			}

			logger(const logger&) = delete;
			logger& operator= (const logger&) = delete;

		public:
			static logger& instance();

			// Adds default log target with given threshold level
			static void default_init(severity_level p_lvl = severity_level::debug);		
			static void add_target(ut::observer_ptr<log_target> p_target);

		public:			
			void operator+= (const log_entry& p_entry);

		public:
			void lock();
			void unlock();

		private:
			void _add_target(ut::observer_ptr<log_target> p_target);

		private:
			std::vector<ut::observer_ptr<log_target>>		m_Targets;		// Non-owning
			std::recursive_mutex							m_Mtx;
	};
}



// Log Macros - internal
#define NS() lg
#define NS_UTIL() ut
#define LOGGER() ::NS()::logger::instance()
#define LOG() LOGGER() += ::NS()::log_entry(__FILE__, __LINE__)
#define LOG_BARE() LOGGER() += ::NS()::log_entry(__FILE__, __LINE__, true)
#define LOG_LVL_BASE( _level, _clr ) LOG() << ::NS()::severity_level::_level << ::NS_UTIL()::console_color::_clr
#define LOG_IF_BASE( _expr, _logexpr ) if(_expr) _logexpr 
#define LOG_EXCEPT_BASE( _logexpr ) _logexpr << "An exception was thrown: "
#define LOG_FMT_BASE( _logexpr, _fmtstr, ...) _logexpr << ::NS_UTIL()::format(_fmtstr, __VA_ARGS__)
#define MACRO_WRAP_BASE( _expr ) do { _expr } while(0)
#define LOG_BARE_FMT( _fmtstr, ... ) LOG_FMT_BASE( LOG_BARE(), _fmtstr, __VA_ARGS__ )
// ---

// Log Macros - basic
#define LOG_F() LOG_LVL_BASE(fatal,		bright_red)
#define LOG_E() LOG_LVL_BASE(error,		bright_red)
#define LOG_W() LOG_LVL_BASE(warning,	bright_yellow)
#define LOG_I() LOG_LVL_BASE(info,		bright_white)
#define LOG_D() LOG_LVL_BASE(debug,		bright_cyan)
// ---

// Log Macros - conditional
#define LOG_F_IF( _expr ) LOG_IF_BASE( (_expr), LOG_F() )
#define LOG_E_IF( _expr ) LOG_IF_BASE( (_expr), LOG_E() )
#define LOG_W_IF( _expr ) LOG_IF_BASE( (_expr), LOG_W() )
#define LOG_I_IF( _expr ) LOG_IF_BASE( (_expr), LOG_I() )
#define LOG_D_IF( _expr ) LOG_IF_BASE( (_expr), LOG_D() )
// ---

// Log Macros - assert
#define LOG_ASSERT_EX( _expr, _msg ) MACRO_WRAP_BASE( if(!_expr){ LOG_F() << "Assertion failed: "  #_expr << " " << _msg; std::terminate(); } )
#define LOG_ASSERT( _expr ) LOG_ASSERT_EX( (_expr), "" )
// ---

// Log Macros - exception
#define LOG_F_EXCEPT() LOG_EXCEPT_BASE( LOG_F() )
#define LOG_W_EXCEPT() LOG_EXCEPT_BASE( LOG_W() )
#define LOG_E_EXCEPT() LOG_EXCEPT_BASE( LOG_E() )
// ---

// Log macros - format
#define LOG_F_FMT( _fmtstr, ... ) LOG_FMT_BASE( LOG_F(), _fmtstr, __VA_ARGS__ )
#define LOG_W_FMT( _fmtstr, ... ) LOG_FMT_BASE( LOG_W(), _fmtstr, __VA_ARGS__ )
#define LOG_E_FMT( _fmtstr, ... ) LOG_FMT_BASE( LOG_E(), _fmtstr, __VA_ARGS__ )
#define LOG_D_FMT( _fmtstr, ... ) LOG_FMT_BASE( LOG_D(), _fmtstr, __VA_ARGS__ )
#define LOG_I_FMT( _fmtstr, ... ) LOG_FMT_BASE( LOG_I(), _fmtstr, __VA_ARGS__ )
// ---

// Log Macros - conditional format
#define LOG_F_FMT_IF( _expr, _fmtstr, ... ) LOG_IF_BASE( (_expr), LOG_F_FMT( _fmtstr, __VA_ARGS__ ) )
#define LOG_E_FMT_IF( _expr, _fmtstr, ... ) LOG_IF_BASE( (_expr), LOG_E_FMT( _fmtstr, __VA_ARGS__ ) )
#define LOG_W_FMT_IF( _expr, _fmtstr, ... ) LOG_IF_BASE( (_expr), LOG_W_FMT( _fmtstr, __VA_ARGS__ ) )
#define LOG_I_FMT_IF( _expr, _fmtstr, ... ) LOG_IF_BASE( (_expr), LOG_I_FMT( _fmtstr, __VA_ARGS__ ) )
#define LOG_D_FMT_IF( _expr, _fmtstr, ... ) LOG_IF_BASE( (_expr), LOG_D_FMT( _fmtstr, __VA_ARGS__ ) )
// ---

// Log macros - MT support
#define LOG_LOCK() MACRO_WRAP_BASE(logger().lock();)
#define LOG_UNLOCK() MACRO_WRAP_BASE(logger().unlock();)

#define LOG_BEGIN() LOG_LOCK()
#define LOG_END() LOG_UNLOCK()
// ---
