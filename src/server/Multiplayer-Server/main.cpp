#include "http.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

const int DEFAULT_BUFLEN = 512;
const char* SERVER_PORT = "3000";
const char* WS_PORT = "7000";

int __cdecl main(void)
{
	HTTPServer server = createServer(SERVER_PORT);
	initServer(&server);

	int iResult;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Receive until the peer shuts down the connection
	do {
		iResult = recv(server.client_socket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			// Echo the buffer back to the sender
			iSendResult = onReceive(server.client_socket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				return onServerError(&server, "Send failed!");
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0) {
			printf("Connection closing...\n");
			break;
		}
		else {
			return onServerError(&server, "Receive failed!");
		}
	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(server.client_socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		return onServerError(&server, "Shutdown failed!");
	}

	// cleanup
	closeServer(&server);
	return 0;
}