#include <gctypes.h>
#include "../main.h"

#define INADDR_ANY (0)
#define AF_INET	(2)
#define AF_INET6 (23)
#define SOCK_STREAM	(1)
#define SOL_SOCKET (-1)
#define SOCK_DGRAM (2)

#define IPPROTO_IP (0)
#define IPPROTO_TCP (6)
#define IPPROTO_UDP (17)

#define SO_REUSEADDR    0x0004
#define SO_NONBLOCK     0x1016

#define ESUCCESS        0
#define ENODATA         1
#define EISCONN         3
#define EWOULDBLOCK     6
#define EALREADY        10
#define EAGAIN          EWOULDBLOCK
#define EINVAL          11
#define ENOMEM          18
#define EINPROGRESS     22

#define MSG_DONTWAIT    0x0020

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

// Struct to sync states across multiplayer. Some technicalities:
// - The server constantly sends their gravity, position, velocity etc.
// - The client doesn't die unless the server says so.
// - If the client doesn't get certain packages, the physics will be synced later.

// TODO: One user could be running faster due to having more frames...
// - Idea: Could implement a system where the front runner gets slowed down so that the other player can catch up. Can't set players back since one player might've made a new obstacle so the grid would differ.

struct gameStateStruct {
	u64 levelSeed; // Syncs the RNG seed used for level generation
	unsigned char infectionLevel; // Character Selected
	unsigned char scrollSpeed; // Scroll Speed
	unsigned int health;
	unsigned int score;
	bool hostCollided;
	// Physics and controls
	int yPosition;
	int upVelocity;
	int downVelocity;

	int blocksTraversed;
	float subPixelScrollOffset;
};

void printStatusText(char* text) {
	drawTextEx(0, 66, 10, 0xF4424200, "Network Status:", true, 2, 2);
	drawTextEx(0, 66, 14, 0xF4424200, text, true, 2, 2);
}

void printDebugNumber(int number) {
	char text[20];
	itoa(number, text, 10);
	drawTextEx(0, 66, 10, 0xF4424200, "Network Debug Number:", true, 2, 2);
	drawTextEx(0, 66, 14, 0xF4424200, text, true, 2, 2);
	finishFrame();
}

struct sockaddr_in socket_addr, other_addr;
char recvBuffer[sizeof(struct gameStateStruct)];
char pingBuffer[] = "PING";

int client_socket = -1;
bool establishedConnection = false;

bool initializeSocket(bool initializeHost) {
	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	printStatusText("Used potatoes to create a new power socket!");
	if (client_socket < 0)	return false;

	memset(&socket_addr, 0, sizeof(socket_addr));
	memset(&other_addr, 0, sizeof(other_addr));
	u32 enableOpt = 1;
	setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enableOpt, sizeof(enableOpt));
	printStatusText("Genetically modified the potato to be non-blocking...");
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (initializeHost) {
		socket_addr.sin_port = htons(8890); // The server port.
		other_addr.sin_port = htons(8891); // The client receiving port.

		// Server listening socket
		int retValue = bind(client_socket, &socket_addr, sizeof(socket_addr));
		if (retValue < 0) return false;
		printStatusText("Strapped some potatoes together for more power!!!");
		memset(pingBuffer, 0, sizeof(pingBuffer));
	}
	else {
		socket_addr.sin_port = htons(8891); // The client receiving port.
		other_addr.sin_port = htons(8890); // The server port.
		other_addr.sin_family = AF_INET;

		// Client listening socket
		int retBindValue = bind(client_socket, &socket_addr, sizeof(socket_addr));
		if (retBindValue < 0) return false;
		printStatusText("Strapped some potatoes together for more power!!!");

		// Set the server IP
		int retPtonValue = inet_pton(AF_INET, ipAddress, &other_addr.sin_addr);
		if (retPtonValue < 0) return false;
	}
	return true;
}


// Establish method
// Host: Wait for client ping, if the ping is received, start the game...
// Client: Continuely send ping message (ping packets could drop with UDP), after it receives the host's gamestate, start the game...

bool establishConnection(struct gameStateStruct gameState) {
	if (currScreen == GAMEPLAY_HOST) {
		printStatusText("Waiting for a client to connect...");
		int connectedAddrLen = sizeof(socket_addr);
		recvfrom(client_socket, pingBuffer, sizeof(pingBuffer), MSG_DONTWAIT, &other_addr, &connectedAddrLen);
		int lastSocketError = socketlasterr();
		if (lastSocketError != EWOULDBLOCK) {
			return true;
		}
	}
	else if (currScreen == GAMEPLAY_CLIENT) {
		printStatusText("Waiting for server to respond to ping...");
		int connectedAddrLen = sizeof(socket_addr);
		sendto(client_socket, pingBuffer, sizeof(pingBuffer), 0, &other_addr, sizeof(other_addr));
		recvfrom(client_socket, &gameState, sizeof(struct gameStateStruct), MSG_DONTWAIT, &socket_addr, &connectedAddrLen);
		int lastSocketError = socketlasterr();
		if (lastSocketError != EWOULDBLOCK) {
			return true;
		}
	}
	return false;
}

void syncStates(struct gameStateStruct gameState) {
	if (currScreen == GAMEPLAY_HOST) {
		sendto(client_socket, &gameState, sizeof(struct gameStateStruct), 0, &other_addr, sizeof(other_addr));
	}
	else if (currScreen == GAMEPLAY_CLIENT) {
		int connectedAddrLen = sizeof(socket_addr);
		recvfrom(client_socket, recvBuffer, sizeof(struct gameStateStruct), MSG_DONTWAIT, &socket_addr, &connectedAddrLen);
		int lastSocketError = socketlasterr();
		if (lastSocketError != EWOULDBLOCK) {
			memcpy(&gameState, recvBuffer, connectedAddrLen);
		}
	}
}

struct gameStateStruct gameState;