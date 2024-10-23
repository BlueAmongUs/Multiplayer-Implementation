#include <iostream>
#include <string.h>
#include "http.h"
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

static string createResponse(int* sendlen) {
	string response = response_head;
	string body = response_body;

	string content_length_str = std::to_string(body.length());
	response.append(content_length_str);
	response.append("\r\n\r\n");
	response.append(body);

	*sendlen = (int)response.length() + 1; // \0
	
	return response;
}

int onReceive(HTTPServer* server, char* buffer, int len, int flags)
{
	int sendlen = 0;
	string str_res = createResponse(&sendlen);
	const char* response = str_res.c_str();
	std::cout << "Response: " << std::endl << response << std::endl;

	int result = send(server->client_socket, response, sendlen, flags);
	return result;
}

int initServer(HTTPServer* server)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ServerSocket = INVALID_SOCKET;
    server->client_socket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed iwht error " << iResult << std::endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, server->port, &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ServerSocket = createSocket(result);
    if (ServerSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        return onServerError(server, "Socket failed!");
    }

    // Setup the TCP listening socket
    iResult = bind(ServerSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR) {
        return onServerError(server, "Bind failed!");
    }

    iResult = listen(ServerSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        return onServerError(server, "Listen failed!");
    }


    // Accept a client socket
    server->client_socket = accept(ServerSocket, NULL, NULL);
    if (server->client_socket == INVALID_SOCKET) {
        return onServerError(server, "Accept failed!");
    }

    // No longer need server socket
    closesocket(ServerSocket);

	return 0;
}

HTTPServer createServer(const char* port)
{
    HTTPServer http_server = {};
    http_server.client_socket = INVALID_SOCKET;
    http_server.port = port;
    return http_server;
}

SOCKET createSocket(addrinfo* address_info)
{
    return socket(
        address_info->ai_family, 
        address_info->ai_socktype, 
        address_info->ai_protocol
    );
}

int closeServer(HTTPServer* server)
{
    int close_code = 0;
    if (server->client_socket != INVALID_SOCKET) {
        closesocket(server->client_socket);
    }
    else {
        close_code = 1;
    }
    WSACleanup();
    return close_code;
}

int onServerError(HTTPServer* server, const char* msg)
{
    std::cout << msg << " " << WSAGetLastError() << std::endl;
    closeServer(server);
    return 1;
}
