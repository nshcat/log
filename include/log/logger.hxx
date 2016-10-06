// logger: Main logger class
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

#include <vector>
#include <string>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <atomic>
#include <ut/observer_ptr.hxx>
#include <ut/format.hxx>

#include "log_entry.hxx"

namespace lg
{
	class log_target;

	class logger
	{
		using container_type = std::vector<ut::observer_ptr<log_target>>;
		using queue_type = std::queue<log_entry>;
		
		private:
			logger();
			~logger();

			logger(const logger&) = delete;
			logger& operator= (const logger&) = delete;

		public:
			// Retrieve global logger instance.
			static logger& instance();

			// Initializes and adds default log target with given threshold level
			static void default_init(severity_level p_lvl = severity_level::debug);	

			// Initialize logger without any log targets
			static void null_init();
			
			// Add custom log target.
			static void add_target(ut::observer_ptr<log_target> p_target);

		public:
			// Queue new log entry for later processing and dispatching.
			void operator+= (log_entry&& p_entry);

		public:
			// Begin LOCK/UNLOCK block
			void lock();
			
			// End LOCK/UNLOCK block. Will also notify the worker thread
			// to make sure all data will be written after a block.
			void unlock();

		private:
			void _add_target(ut::observer_ptr<log_target> p_target);
			
		private:
			// Worker thread function
			void do_work();
			
			// Notify worker thread of new data
			void notify();
			
			// Request worker thread shutdown
			void kill_thread();
			
			// Dispatch log entry to all registered log targets
			void dispatch(const log_entry&);

		private:
			std::atomic_bool m_Empty{true};		// Whether the logger has no targets
			container_type m_Targets;			// Non-owning pointers to log targets
			queue_type m_WorkQueue;				// Queue that holds all log entry data
			queue_type m_TempQueue;				// Queue that holds queue tail during dispatch
			
		private:
			bool m_IsLocked{false};				// Whether we currently are in a LOCK/UNLOCK block
			std::recursive_mutex m_Mtx;			// Access mutex for producer threads
			std::condition_variable m_WorkCv;	// Used to notify the consumer thread of new data
			std::mutex m_CvMtx;					// Mutex used by the condition_variable
			
			bool m_HasWork{false};				// Whether there is work pending to be processed
			std::atomic_bool m_ShouldStop{false};	// Whether the worker thread is requested to stop
			std::thread m_Worker;				// Worker thread		
			
	};
}



// Log Macros - internal
#define NS() lg
#define NS_UTIL() ut
#define LOGGER() ::NS()::logger::instance()
#define LOG() LOGGER() += ::NS()::log_entry(__FILE__, __LINE__)
#define LOG_BARE() LOGGER() += ::NS()::log_entry(__FILE__, __LINE__, true)
#define LOG_BARE_BASE( _level, _clr ) LOG_BARE() << ::NS()::severity_level::_level << ::NS_UTIL()::console_color::_clr
#define LOG_LVL_BASE( _level, _clr ) LOG() << ::NS()::severity_level::_level << ::NS_UTIL()::console_color::_clr
#define LOG_IF_BASE( _expr, _logexpr ) if(_expr) _logexpr 
#define LOG_EXCEPT_BASE( _logexpr ) _logexpr << "An exception was thrown: "
#define LOG_FMT_BASE( _logexpr, _fmtstr, ...) _logexpr << ::NS_UTIL()::sprintf(_fmtstr, __VA_ARGS__)
#define MACRO_WRAP_BASE( _expr ) do { _expr } while(0)
#define LOG_BARE_FMT( _fmtstr, ... ) LOG_FMT_BASE( LOG_BARE(), _fmtstr, __VA_ARGS__ )
#define LOG_BARE_EMPTY() MACRO_WRAP_BASE(LOG_BARE();)
// ---

// Log Macros - basic
#define LOG_F() LOG_LVL_BASE(fatal,		bright_red)
#define LOG_E() LOG_LVL_BASE(error,		bright_red)
#define LOG_W() LOG_LVL_BASE(warning,	bright_yellow)
#define LOG_I() LOG_LVL_BASE(info,		bright_white)
#define LOG_D() LOG_LVL_BASE(debug,		bright_cyan)
// ---

// Log Macros - bare
#define LOG_F_BARE() LOG_BARE_BASE(fatal,		bright_red)
#define LOG_E_BARE() LOG_BARE_BASE(error,		bright_red)
#define LOG_W_BARE() LOG_BARE_BASE(warning,		bright_yellow)
#define LOG_I_BARE() LOG_BARE_BASE(info,		bright_white)
#define LOG_D_BARE() LOG_BARE_BASE(debug,		bright_cyan)
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
#define LOG_LOCK() MACRO_WRAP_BASE(LOGGER().lock();)
#define LOG_UNLOCK() MACRO_WRAP_BASE(LOGGER().unlock();)

#define LOG_BEGIN() LOG_LOCK()
#define LOG_END() LOG_UNLOCK()
// ---
