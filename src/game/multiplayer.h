#include <gctypes.h>
#include "main.h"

#define INADDR_ANY (0)
#define AF_INET	(2)
#define SOCK_STREAM	(1)
#define SOL_SOCKET (-1)
#define SOCK_DGRAM (2)

#define IPPROTO_IP (0)
#define IPPROTO_TCP (6)
#define IPPROTO_UDP (17)

#define SO_REUSEADDR    0x0004
#define SO_NONBLOCK     0x1016

#define ENODATA         1
#define EISCONN         3
#define EWOULDBLOCK     6
#define EALREADY        10
#define EAGAIN          EWOULDBLOCK
#define EINVAL          11
#define ENOMEM          18
#define EINPROGRESS     22

#define htonl(x) x
#define htons(x) x
#define ntohl(x) x
#define ntohs(x) x

struct in_addr {
	unsigned int s_addr;
};
struct sockaddr_in {
	short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
};

struct sockaddr
{
	unsigned short sa_family;
	char sa_data[14];
};

int client_socket = -1;

void printStatusText(char* text) {
	OSScreenClearBufferEx(0, 0);
	OSScreenClearBufferEx(1, 0);
	drawTextEx(0, 66, 10, 0xF4424200, "Waiting for someone to connect...", true, 2, 2);
	drawTextEx(0, 66, 14, 0xF4424200, text, true, 2, 2);
	finishFrame();
}

bool initializeSocket = true;
struct sockaddr_in socket_addr;

#define RECVBUFFER_SIZE (20)
char recvBuffer[RECVBUFFER_SIZE];

bool waitForConnection() {
	if (initializeSocket) {
		client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
		printStatusText("Used potatoes to create a new power socket!");
		if (client_socket < 0)	return false;

		u32 enableOpt = 1;
		setsockopt(client_socket, SOL_SOCKET, SO_NONBLOCK, &enableOpt, sizeof(enableOpt));
		setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enableOpt, sizeof(enableOpt));
		printStatusText("Genetically modified the potato to be non-blocking...");
		memset(&socket_addr, 0, sizeof(socket_addr));
		socket_addr.sin_family = AF_INET;
		socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		socket_addr.sin_port = htons(8891);

		s32 retValue;
		retValue = bind(client_socket, &socket_addr, sizeof(socket_addr));
		if (retValue < 0) return false;
		printStatusText("Strapped some potatoes together for more power!!!");
		initializeSocket = false;
	}

	char *bufferStatus[10];
	int recvStatus = recv(client_socket, recvBuffer, RECVBUFFER_SIZE, );
	itoa(recvStatus, bufferStatus, 10);
	printStatusText(*bufferStatus);
}