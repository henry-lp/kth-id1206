#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <assert.h>

#define PORT 8080
#define MAX 512

int main(void) {
	int sock;
	char buffer[MAX];

	/* A socket is created */
	assert((sock = socket(AF_INET, SOCK_DGRAM, 0)) != -1);

	struct sockaddr_in name;

	name.sin_family = AF_INET;
	name.sin_port = htons(PORT);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	assert(bind(sock, (struct sockaddr *) &name, sizeof(name)) != -1);

	assert(listen(sock, 5) != -1);

	int cfd;

	assert((cfd = accept(sock, NULL, NULL)) != -1);

	printf("Server: recieved: %s\n", buffer);
	printf("Server: from destination %s %d\n", inet_ntoa(client.sin_adddr), nthos(client));
}
