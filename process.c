#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv){
	char processName[32] = argv[1];
	int readyTime = atoi(argv[2]);
	int execTime = atoi(argv[3]);
	return 0;
}