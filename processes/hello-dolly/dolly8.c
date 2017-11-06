// the group
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

	int pid = fork();

	if (pid == 0) {
		int child = getpid();
		printf("i am the child %d in group %d\n", child, getpgid(child));
	} else {
		int parent = getpid();
		printf("i am the parent %d in group %d\n", parent, getpgid(parent));
		wait(NULL);
	}

	return 0;
}
