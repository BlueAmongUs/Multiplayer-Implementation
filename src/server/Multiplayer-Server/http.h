#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>

int onReceive(SOCKET socket, char* buf, int len, int flags);

struct HTTPServer
{
	SOCKET socket;
	const char* DEFAULT_PORT;
	char buff;
	int buff_len;
};

int initServer(const char* port, SOCKET* client_server);