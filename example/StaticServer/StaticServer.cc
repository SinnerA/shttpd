#include "StaticServer.hh"
#include "MimeType.hh"
#include <fstream>
#include <regex>
#include <algorithm>

void
StaticServer::handleRequest(RequestPtr req, ResponsePtr res) 
{
	if(req->method() != "GET") {				/**< 只允许GET请求 */
		res->status() = Response::not_implemented;
		return;
	}
	std::string file_name = doc_root;
	std::string path = req->path();
	if(path[path.size()-1] == '/')
		path += "index.html";
	file_name += path;
	std::ifstream file(file_name);
	if(!file) {
		res->status() = Response::not_found;
		return;
	}
	res->setMimeType(guessMimeType(file_name));
	res->out() << file.rdbuf();
}

int 
main(int argc, char* argv[])
{
	try {
		asio::io_service io_service;
		Server server(io_service, "8888");			/**< 在8888端口监听 */
		server.addHandler("/", new StaticServer(&server));
		server.run(10);						/**< 给io_service 10个线程 */
	} catch(std::exception& e) {
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}
