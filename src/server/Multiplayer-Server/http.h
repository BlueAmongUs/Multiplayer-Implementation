#pragma once
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>

struct HTTPServer
{
	SOCKET client_socket;
	const char* port;
};

int onReceive(HTTPServer* server, char* buf, int len, int flags);

int initServer(HTTPServer* server);

HTTPServer createServer(const char* port);

SOCKET createSocket(addrinfo* address_info);
int closeServer(HTTPServer* server);
int onServerError(HTTPServer* server, const char* msg);