#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <assert.h>

#define MAX 512
#define TEST "This is a TeSt to SEE if iT wORks"
#define CLIENT "help"

int main(void) {
	int sock;
	char buffer[MAX];

	/* A socket is created */
	assert((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) != -1);

	struct sockaddr_un name = {AF_UNIX, CLIENT};

	assert(connect(sock, (struct sockaddr *)&name, sizeof(name)) != -1);

	struct sockaddr_un server = {AF_UNIX, SERVER};
	int size = sizeof(TEST);

	assert(sendto(sock, TEST, n, 0, (struct sockaddr *) &server, size) != -1);
	n = recvfrom(sock, buffer, MAX-1, 0, (struct sockaddr *) &server, &size);
	if(n == -1) {
		perror("server");
	}

	buffer[n] = 0;
	printf("Client received: %s\n, buffer);
	unlink(CLIENT);
	exit(0);
}
