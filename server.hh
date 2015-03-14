#pragma once

#include <boost/asio.hpp>
#include <string>
#include "connection.hh"
#include "RequestHandler.hh"
#include "request.hh"
#include "response.hh"

/// The top-level class of the HTTP Server.
class Server {
public:
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;

	/// Construct the Server to listen on the specified TCP address and port, and
	/// serve up files from the given directory.
	explicit Server(boost::asio::io_service& service, const std::string& port);

	/// Run the Server's io_service loop.
	void run();

	void addHandler(const std::string& path, RequestHandlerPtr handle) {
		request_handler_.addSubHandler(path, handle);
	}

	void deliverRequest(RequestPtr req, ResponsePtr rep) {
		request_handler_.handleRequest(req, rep);
	}

	boost::asio::io_service& service() {
		return service_;
	}
	
	void post(const std::function<void(void)>& func) {
		service_.post(func);
	}

private:

	/// Perform an asynchronous accept operation.
	void do_accept();

	/// Wait for a request to stop the Server.
	void do_await_stop();

	/// The io_service used to perform asynchronous operations.
	boost::asio::io_service& service_;

	/// The signal_set is used to register for process termination notifications.
	boost::asio::signal_set signals_;

	/// Acceptor used to listen for incoming connections.
	boost::asio::ip::tcp::acceptor acceptor_;

	/// The connection manager which owns all live connections.
//	ConnectionManager connection_manager_;

	/// The next socket to be accepted.
	boost::asio::ip::tcp::socket socket_;

	/// The handler for all incoming requests.
	RequestHandler request_handler_;

};
