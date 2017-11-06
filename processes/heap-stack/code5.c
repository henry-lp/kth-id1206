// the heap
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "This is a global sring";
const int read_only = 12345;

int main() {
	int pid = getpid();

	char *heap = malloc(20);

foo:
	printf("proccess id: %d\n", pid);
	printf("global string: %p\n", &global);
	printf("the code: %p\n", &&foo);
	printf("read only: %p\n", &read_only);

	printf("the heap variable at: %p\n", &heap);
	printf("pointing to: %p\n", heap);


	printf("\n\n /proc/%d/maps \n\n", pid);
	char command[50];
	sprintf(command, "cat /proc/%d/maps", pid);
	system(command);

	return 0;
}
