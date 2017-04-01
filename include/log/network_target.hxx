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

/*	PROTOCOL:
 *	
 *	[u32]	Length of message string
 *	[u32]	Length of source string
 *	[u64]	Timestamp (time_t)
 *	[u32] 	Level
 *	[u32] 	Is Bare? (0x1 or 0x0)
 *	N*[u8]	String payload
 *	M*[u8]	Source payload
 */


#pragma once

#include <string>

#include "log_target.hxx"
#include "log_entry.hxx"

namespace lg
{
	class network_target
		: public log_target
	{
		using handle_type = void*;

		public:
			network_target()
				: log_target(severity_level::debug)
			{}

			network_target(severity_level p_lvl, const ::std::string& p_host, const ::std::string& p_port, const ::std::string& p_src = ::std::string{});
			~network_target();

			network_target(const network_target&) = delete;
			network_target(network_target&&) = default;

			network_target& operator=(const network_target&) = delete;
			network_target& operator=(network_target&&) = default;

		public:
			virtual void write(const log_entry& entry) override;

		private:
			auto connect() -> void;

		private:
			::std::string m_Host;
			::std::string m_Port;
			::std::string m_Src;
			handle_type m_InternalData{nullptr};
	};
}
