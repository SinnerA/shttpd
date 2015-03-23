#include "TcpConnection.hh"
#include <utility>
#include <asio.hpp>
#include "log.hh"
#include "ptrs.hh"

void 
TcpConnection::stop()
{
	socket_.close();
}

void 
TcpConnection::async_read_until(const std::string& delim, 
	std::function<void(const asio::error_code &, size_t)> handler)
{
	asio::async_read_until(socket_, readBuffer(), delim, [=](auto&& e, auto&& s) {
		handler(e, s);
	});
}
	
void 
TcpConnection::async_write(std::function<
	void(const asio::error_code&, size_t)> handler)
{
	asio::async_write(socket_, writeBuffer(), std::bind(
		[handler](const asio::error_code& e, size_t n, ConnectionPtr) {
			handler(e, n);
		}, 
		std::placeholders::_1, std::placeholders::_2, shared_from_this())); /**< 防止过早析构 */
}
