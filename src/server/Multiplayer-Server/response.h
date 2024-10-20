#pragma once
#include <WinSock2.h>

int onReceive(SOCKET socket, char* buf, int len, int flags);

struct HTTPServer
{
	SOCKET socket;
	const char* DEFAULT_PORT;
	char buff;
	int buff_len;
};

HTTPServer makeServer();
