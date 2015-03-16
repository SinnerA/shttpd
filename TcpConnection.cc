#include "TcpConnection.hh"
#include <utility>
#include <boost/asio.hpp>
#include "log.hh"

void 
TcpConnection::stop()
{
	socket_.close();
}

void 
TcpConnection::async_read_until(const std::string& delim, 
	std::function<void(const boost::system::error_code &, size_t)> handler)
{
	boost::asio::async_read_until(socket_, buffer(), delim, handler);
}
	
void 
TcpConnection::async_write(std::function<
	void(const boost::system::error_code&, size_t)> handler)
{
	boost::asio::async_write(socket_, buffer(), std::bind(
		[handler](const boost::system::error_code& e, size_t n, ConnectionPtr) {
			handler(e, n);
		}, 
		std::placeholders::_1, std::placeholders::_2, shared_from_this())); /**< 防止过早析构 */
}