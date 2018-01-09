#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <assert.h>

#define MAX 512
#define TEST "Hello, hello"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(void) {
	int sock;
	char buffer[MAX];

	/* A socket is created */
	assert((sock = socket(AF_INET, SOCK_DGRA, 0)) != -1);

	struct sockaddr_in name;

	name.sin_family = AF_INET;
	name.sin_port = 0;
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	assert(bind(sock, (struct sockadrr *) &name, sizeof(name)) != -1);

	sturct sockadrr_in server;

	server.sin_family = AF_INET;
	server.sin_port = hton(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(SERVER_IP);
