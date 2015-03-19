#include <iostream>
#include <string>
#include "server.hh"

struct TestHandler : public RequestHandler {
	using RequestHandler::RequestHandler;

	void handleRequest(RequestPtr req, ResponsePtr rep) override {
		rep->out() << "it's TestHander::handleRequest" << std::endl;
		rep->out() << "method: " << req->getMethod() << std::endl;
		rep->out() << "uri: " << req->getUri() << std::endl;
		rep->out() << "version: " << req->getVersion() << std::endl;
		for(auto&& h : req->headerMap())
			rep->out() << h.name << ": " << h.value << std::endl;
	}
};

int 
main(int argc, char* argv[])
{
	try {
		asio::io_service io_service;
		Server server(io_service, "8888", "9999");
		server.addHandler("/test", RequestHandlerPtr(new TestHandler(&server)));
		server.run(10);
	} catch(std::exception& e) {
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}
