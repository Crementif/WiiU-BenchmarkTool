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

u32 sockOptionEnable = 1;
int sockAddressLength = sizeof(struct sockaddr_in);
char pingBuffer[5] = "PING";
bool establishedConnection = true;
bool sendLastMessage = false;

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

struct sockaddr_in client_destination_addr, client_receiving_addr;
int client_destination_socket, client_receiving_socket = 1;

bool initializeHost() {
	// Initialize server socket and address
	client_receiving_addr.sin_port = htons(8890);
	client_receiving_addr.sin_family = AF_INET;
	client_receiving_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((client_receiving_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) return false;
	if (bind(client_receiving_socket, &client_receiving_addr, sizeof(struct sockaddr_in)) < 0) return false;
	setsockopt(client_receiving_socket, SOL_SOCKET, SO_REUSEADDR, &sockOptionEnable, sizeof(sockOptionEnable));

	memset(pingBuffer, 0, sizeof(pingBuffer)); // Set the ping buffer to 0
	return true;
}

bool initializeClient() {
	// Initialize receiving socket and address
	client_receiving_addr.sin_port = htons(8891);
	client_receiving_addr.sin_family = AF_INET;
	client_receiving_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((client_receiving_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) return false;
	if (bind(client_receiving_socket, &client_receiving_addr, sizeof(struct sockaddr_in)) < 0) return false;
	// Initialize sending socket and address
	client_destination_addr.sin_port = htons(8890);
	client_destination_addr.sin_family = AF_INET;
	if ((client_destination_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) return false;
	if ((inet_pton(AF_INET, ipAddress, &client_destination_addr.sin_addr)) < 0) return false;
	memcpy(pingBuffer, "PING", sizeof("PING"));
	return true;
}

// Handshake explanation:
// Host: Wait for client ping, if the ping is received, start the game...
// Client: Continuely send ping message, after it receives the host's gamestate, start the game...

struct gameStateStruct gameState;

bool establishConnectionWithClient() {
	recvfrom(client_receiving_socket, pingBuffer, sizeof(pingBuffer), MSG_DONTWAIT, &client_destination_addr, &sockAddressLength);
	if (socketlasterr() != EWOULDBLOCK) {
		client_destination_addr.sin_port = htons(8891);
		return true;
	}
	return false;
}

bool establishConnectionWithHost() {
	sendto(client_destination_socket, pingBuffer, sizeof(pingBuffer), 0, &client_destination_addr, sizeof(struct sockaddr_in));
	recvfrom(client_receiving_socket, &gameState, sizeof(struct gameStateStruct), MSG_DONTWAIT, &client_receiving_addr, &sockAddressLength);
	if (socketlasterr() != EWOULDBLOCK) return true;
	return false;
}

void syncGameState() {
	if (currScreen == GAMEPLAY_HOST && !sendLastMessage) {
		sendto(client_receiving_socket, &gameState, sizeof(struct gameStateStruct), 0, &client_destination_addr, sizeof(struct gameStateStruct));
		if (gameState.hostCollided == true) sendLastMessage = true;
	}
	else if (currScreen == GAMEPLAY_CLIENT) {
		recvfrom(client_receiving_socket, &gameState, sizeof(struct gameStateStruct), MSG_DONTWAIT, &client_receiving_addr, &sockAddressLength);
	}
}