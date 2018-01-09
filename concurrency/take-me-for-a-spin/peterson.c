// peterson's algorithm
// two threads incrementing a shared counter

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

volatile int count = 0;

volatile int request[2] = {0,0};
volatile int turn = 0;

// to take the lock a thread will set its own flag to 1 and then
// wait until either the other thread is not interested or it is
// its turn. Releasing the lock is done by resetting the req flag.
void lock(int id) {
	request[id] = 1;
	int other = 1 - id;
	turn = other;
	while(request[other] == 1 && turn == other) {
		// spin
	}
}

void unlock(int id) {
	request[id] = 0;
}

typedef struct args {
	int inc;
	int id;
} args;

void *increment(void *arg) {
	int inc = ((args*) arg) -> inc;
	int id = ((args*) arg) -> id;

	for(int i = 0; i < inc; i++) {
		lock(id);
		count++;
		unlock(id);
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("usage peterson <inc>\n");
		exit(0);
	}

	int inc = atoi(argv[1]);

	pthread_t one_p, two_p;
	args one_args, two_args;

	one_args.inc = inc;
	two_args.inc = inc;

	one_args.id = 0;
	one_args.id = 1;

	pthread_create(&one_p, NULL, increment, &one_args);
	pthread_create(&two_p, NULL, increment, &two_args);
	pthread_join(one_p, NULL);
	pthread_join(two_p, NULL);

	printf("result is %d\n", count);
	return 0;
}
