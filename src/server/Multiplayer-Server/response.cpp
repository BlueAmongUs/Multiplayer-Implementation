#include <iostream>
#include <string.h>
#include "response.h"
#include <string>

using std::string;

const char* response_body = "<html>"
"<head>"
"<title>Multiplayer Implementaion</title>"
"</head>"
"<body>"
"<h1>Hello, world!</h1>"
"</body>"
"<html>"
"\r\n\r\n";

const char* response_head = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Content-Length: ";

string createResponse(int* sendlen) {
	string response = response_head;
	string body = response_body;

	string content_length_str = std::to_string(body.length());
	response.append(content_length_str);
	response.append("\r\n\r\n");
	response.append(body);

	*sendlen = (int)response.length() + 1; // \0
	
	return response;
}

int onReceive(SOCKET socket, char* buffer, int len, int flags)
{
	int sendlen = 0;
	string strresponse = createResponse(&sendlen);
	const char* response = strresponse.c_str();
	std::cout << response << std::endl;

	int result = send(socket, response, sendlen, flags);
	return result;
}

HTTPServer makeServer()
{
	HTTPServer server;
	server.socket = INVALID_SOCKET;
	SOCKET self_socket = INVALID_SOCKET;

	return HTTPServer();
}
