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

#include <stdexcept>
#include <mutex>

#include "console_target.hxx"
#include "logger.hxx"
#include "log_target.hxx"

namespace lg
{
	using namespace std::chrono_literals;

	logger::logger()
		: m_Worker{ &logger::do_work, this }
	{
		
	}
	
	logger::~logger()
	{
		// Signal worker thread to stop and block until done
		kill_thread();
	}
	
	
	// Signal the work thread to stop
	void logger::kill_thread()
	{
		// Signal thread to stop
		m_ShouldStop.store(true);
		
		// Wake up thread
		notify();
		
		// Block until done
		m_Worker.join();
	}
	
	// Notify worker thread about new data
	void logger::notify()
	{
		{
			::std::lock_guard<std::mutex> lck(m_CvMtx);
			m_HasWork = true;
		}
		
		m_WorkCv.notify_one();
	}
	
	
	// Worker thread function
	void logger::do_work()
	{
		// Maximum duration to wait on condition variable.
		const auto t_maxWait = 100ms;
		
		// Local work indicator. Is used after lock is released.
		bool t_hasWork{};
		
		// Loop until stop is requested
		while(!m_ShouldStop.load())
		{
			// Critical section begin
			{
				std::unique_lock<std::mutex> lck(m_CvMtx);
											 
				// Block until user thread notifies about new work.
				m_WorkCv.wait_for(lck, t_maxWait);
						
				// Check if there really is work, since the block
				// will periodically release even if there was no notification.
				// This is done to process work items that were added in the brief period
				// inbetween cv lock release and next cv wait (aka. time spent processing items).
				// This is unfortunately a small inconvienience introduced by trying to spend as 
				// little time as possible on producer side.
				// This also happens when a worker thread stop was requested.
				if(m_HasWork)
				{
					// Lock mutex. This stops user threads from pushing new entries while we are
					// dispatching.
					std::lock_guard<std::recursive_mutex> qlck(m_Mtx);
							
					// Indicate that there indeed was data.
					// We can't use m_HasWork after releasing the lock for obvious reasons.
					t_hasWork = true;
						
					// Move all elements to temporary queue.
					// This way we lock as briefly as possible, since dispatching could involve I/O.
					while(!m_WorkQueue.empty())
					{
						m_TempQueue.emplace(::std::move(m_WorkQueue.front()));
						m_WorkQueue.pop();
					}
					
					// Reset work indicator. Everything was dispatched.
					m_HasWork = false;
				}			
			}
			// Critical section end
			
			// Lock is now released. If there was work, dispatch it now.
			if(t_hasWork)
			{			
				// Lock is released. We can now take as much time as needed to dispatch entries.
				while(!m_TempQueue.empty())
				{
					dispatch(m_TempQueue.front());
					m_TempQueue.pop();
				}
				
				// Reset local work indicator
				t_hasWork = false;
			}
		}
	}
	
	// Write entry to all registered log targets
	void logger::dispatch(const log_entry& p_entry)
	{
		if(m_Empty.load())
			return;
		else
		{
			// Iterate through all targets to dispatch entry
			for (auto& target : m_Targets)
			{
				// Log only if verbosity level does not exclude entry level
				if (target->level() >= p_entry.level())
				{
					target->write(p_entry);
				}
			}
		}
	}
	
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
	
	void logger::null_init()
	{
		// Make sure that the logger constructor is called
		instance()._add_target(nullptr);
	}

	void logger::_add_target(ut::observer_ptr<log_target> target)
	{
		if(target)
		{
			m_Targets.push_back(target);
			m_Empty.store(false);
		}
	}

	void logger::add_target(ut::observer_ptr<log_target> target)
	{
		instance()._add_target(target);
	}

	// Insert log entry
	void logger::operator+= (log_entry&& p_entry)
	{
		// Avoid locking when logger is disabled / empty
		if(m_Empty.load())
			return;
		
		// Scope to automatically release mutex lock
		{
			// Lock queue lock and push log entry
			std::lock_guard<std::recursive_mutex> lck(m_Mtx);
			m_WorkQueue.push(std::move(p_entry));
		}
		
		// If we are in a surrounding LOCK/UNLOCK block, notifying here would
		// lead to a deadlock.
		if(!m_IsLocked)
			// Otherwise, notify the working thread that new data is available.
			notify();
	}

	void logger::lock()
	{
		m_Mtx.lock();
		m_IsLocked = true;
	}

	void logger::unlock()
	{
		m_IsLocked = false;
		m_Mtx.unlock();
		
		// We have to notify here to make sure that the working thread is aware of
		// the data that was inserted in the previous LOCK/UNLOCK block.		
		notify();
	}
}