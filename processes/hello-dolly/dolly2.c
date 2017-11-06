// extension
#include <stdio.h>
#include <unistd.h>

int main() {

	int pid = fork();

	if (pid == 0) {
		printf("I am the child: %d\n", getpid());
	} else {
		printf("My child is called %d\n", pid);
	}
	printf("That's it  %d\n", getpid());

	return 0;
}
