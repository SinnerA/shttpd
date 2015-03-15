#include "RequestHandler.hh"
#include <fstream>
#include <sstream>
#include <string>
#include "MimeTypes.hh"
#include "response.hh"
#include "request.hh"
#include "server.hh"

#include <iostream>

RequestHandler::RequestHandler(Server *server)
	: server_(server)
{
}

void 
RequestHandler::handleRequest(RequestPtr req, ResponsePtr rep)
{
  	// Decode url to path.
	std::string request_path;
 
	if(!url_decode(req->getUri(), request_path)) {
		/** TODO: BAD REQUEST */
		std::cout << __FILE__ << __LINE__ << std::endl;
		req->connection()->stop();
    		return;
  	}

	req->setUri(request_path);

  	if(!deliverRequest(req, rep)) {
		rep->setStatus(Response::not_found);		
		return;
  	}
}

bool
RequestHandler::deliverRequest(RequestPtr req, ResponsePtr rep)
{
	std::tuple<std::string, RequestHandlerPtr> best;
	
	for(auto handler : sub_handlers_) {
		size_t cmp_size = std::get<0>(handler).size();
		if(req->getUri().size() >= cmp_size && 
			req->getUri().substr(0, cmp_size) == std::get<0>(handler))
			best = handler;	
	}

	if(std::get<1>(best) == nullptr) {
		std::cout << "uri = " << req->getUri() << std::endl;
		return false;
	}
	std::get<1>(best)->handleRequest(req, rep);
	return true;
}

bool 
RequestHandler::url_decode(const std::string& in, std::string& out)
{
	out.clear();
	out.reserve(in.size());
	for(std::size_t i = 0; i < in.size(); ++i) {
		if(in[i] == '%') {
      			if(i + 3 <= in.size()) {
        			int value = 0;
        			std::istringstream is(in.substr(i + 1, 2));
        			if(is >> std::hex >> value) {
          				out += static_cast<char>(value);
          				i += 2;
        			} else {
          				return false;
        			}
      			} else {
        			return false;
      			}
    		} else if (in[i] == '+') {
      			out += ' ';
    		} else {
      			out += in[i];
    		}
  	}
  	return true;
}
