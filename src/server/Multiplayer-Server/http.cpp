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

int initServer(const char* port, SOCKET* client_socket)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ServerSocket = INVALID_SOCKET;
    *client_socket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ServerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ServerSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ServerSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ServerSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ServerSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocket);
        WSACleanup();
        return 1;
    }


    // Accept a client socket
    *client_socket = accept(ServerSocket, NULL, NULL);
    if (*client_socket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ServerSocket);

	return 0;
}

HTTPServer makeServer()
{
	HTTPServer server;
	server.socket = INVALID_SOCKET;
	SOCKET self_socket = INVALID_SOCKET;

	return HTTPServer();
}
