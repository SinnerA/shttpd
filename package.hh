#pragma once

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <sstream>
#include <utility>
#include "header.hh"
#include "connection.hh"

/// A response to be sent to a client.
class Package
{
public:
	Package(Server *server, ConnectionPtr connection) :
		server_(server), connection_(connection) {}

	virtual ~Package() {}
	std::istream& in() { return body; }
	std::ostream& out() { return body; }
	ConnectionPtr connection() { return connection_; }

	std::vector<std::string> getHeader(std::string h_name) {
		std::vector<std::string> dst_header;
		for(auto h : headers) {
			if(h.name == h_name)
				dst_header.push_back(h.value);
		}
		return std::move(dst_header);
	}

	void addHeader(const std::string& h_name, const std::string& h_value) {
		headers.push_back(header_t{h_name, h_value});
	}

	void setHeader(const std::string h_name, const std::string& h_value) {
		for(auto& h : headers) {
			if(h.name == h_name)
				h.value = h_value;
		}
	}
	
	std::vector<header_t>& headerMap() { return headers; }

protected:
	Server *server_;

private:

  	/// The headers to be included in the response.
	std::vector<header_t> headers;

	/// The content to be sent in the response.
	std::stringstream body;

	ConnectionPtr connection_;	
};
