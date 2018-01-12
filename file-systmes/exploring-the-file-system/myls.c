#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
	if(argc < 2) {
		perror("usage: myls <dir>\n");
		return -1;
	}

	char *path = argv[1];

	DIR *dirp = opendir(path);

	struct dirent *entry;

	while((entry = readdir(dirp)) != NULL) {
		switch(entry -> d_type) {
			case DT_BLK :		// This is a block device
				printf("b:");
				break;
			case DT_CHR :		// This is a character device
				printf("c:");
				break;
			case DT_DIR :		// This is a directory
				printf("d:");
				break;
			case DT_FIFO :		// This is a named pipe
				printf("p:");
				break;
			case DT_LNK :		// This is a symbolic link
				printf("l:");
				break;
			case DT_REG :		// This is a regular file
				printf("f:");
				break;
			case DT_SOCK : 		// This is a UNIX domain socket
				printf("s:");
				break;
			case DT_UNKNOWN :	// The file type is unknown
				printf("u:");
				break;
			}
		printf("\tinode %lu", entry -> d_ino);
		printf("\tname: %s\n", entry -> d_name);
	}
}
