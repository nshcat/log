#define ASIO_STANDALONE 1

#include <limits>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <ut/throwf.hxx>
#include <ut/cast.hxx>
#include <ut/utility.hxx>
#include <asio.hpp>

#include <network_target.hxx>


namespace lg
{
	// The structure holding all objects this class needs.
	// This is done to not expose asio headers, which are quite big,
	// to the outer world.
	struct network_target_data
	{
		network_target_data()
			: 	m_Service{ },
				m_Socket{m_Service}
		{
		}

		~network_target_data() = default;

		asio::io_service m_Service;
		asio::ip::tcp::socket m_Socket;
	};

	
	auto cast(void* p_ptr)
		-> network_target_data*
	{
		return static_cast<network_target_data*>(p_ptr);
	}


	network_target::network_target(severity_level p_lvl, const ::std::string& p_host, const ::std::string& p_port)
		: log_target(p_lvl), m_Host{p_host}, m_Port{p_port}, m_InternalData{ new network_target_data() }
	{
		connect();
	}

	network_target::~network_target()
	{
		// Close socket
		cast(m_InternalData)->m_Socket.close();

		if(m_InternalData)
			delete cast(m_InternalData);
	}

	void network_target::connect()
	{
		auto& t_sock = cast(m_InternalData)->m_Socket;

		// Create tcp resolver and query to resolve hostname
		asio::ip::tcp::resolver t_resolver(cast(m_InternalData)->m_Service);
		asio::ip::tcp::resolver::query t_query(m_Host, m_Port);

		// Do query.
		auto t_begin = t_resolver.resolve(t_query);
		decltype(t_begin) t_end{}; // End iterator has to be default constructed resolver iterator

		asio::error_code t_err = asio::error::host_not_found;
		while(t_err && t_begin != t_end)
		{
			t_sock.close();
			t_sock.connect(*t_begin++, t_err);
		}

		// Query results exhausted, but no connection was made
		if(t_err)
		{
			ut::throwf<::std::runtime_error>(
				"network_target: Failed to connect to remote server \"%s:%s\"",
				m_Host,
				m_Port
			);
		}

		// Everything went well, socket is ready.
	}

	void network_target::write(const log_entry& p_entry)
	{
		// Calculate size of packet
		
		// The string length field is only 4 bytes wide, so we need
		// to truncate the message if needed
		const ::std::size_t t_szStr = ::std::min<::std::size_t>(
			p_entry.message().length(),
			::std::numeric_limits<::std::uint32_t>::max()
		);

		const ::std::size_t t_szFixed = 20; // Fixed part of packet
		const ::std::size_t t_sz = t_szStr + t_szFixed;

		// Create buffer for packet
		::std::ostringstream t_stream{};

		// Write data
		ut::to_bytes_be<::std::uint32_t>(t_stream, t_szStr);
		ut::to_bytes_be<::std::uint64_t>(t_stream, p_entry.time());
		ut::to_bytes_be<::std::uint32_t>(t_stream, ut::enum_cast(p_entry.level()));
		ut::to_bytes_be<::std::uint32_t>(t_stream, (p_entry.bare() ? 1u : 0u));
		
		t_stream.write(p_entry.message().data(), t_szStr);

		const auto t_result = t_stream.str();

		cast(m_InternalData)->m_Socket.write_some(asio::buffer(t_result.data(), t_sz));
	}
}
