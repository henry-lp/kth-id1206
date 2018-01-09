#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ITERATIONS 10
#define BURST 1000

// consumer assumues that there is a pipe called sesame
// and will open this for reading.

int main() {
	/* open pipe for reading */
	int flag = O_RDONLY;
	int pipe = open("sesame", flag);

	for(int i = 0; i < ITERATIONS; i++) {
		for(int j = 0; j < BURST; j++) {
			int buffer[10];
//			read(descr[0], &buffer, 10);
		}
		sleep(1);
	}

	printf("consumer done\n");
	return 0;
}
