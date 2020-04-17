#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#define maxChild 100
int main(){
	char schedulingPolicy[20];
	int processNum;
	int childPID[maxChild];
	scanf("%s%d", schedulingPolicy, &processNum);
	for(int i=1 ; i<=processNum ; i++){
		if((childPID[i] = fork()) == 0){
			char processName[32];
			char readyTime[10];
			char execTime[10];
			scanf("%s%s%s", processName, readyTime, execTime);
			char* const arg[6] = {"./process", processName, readyTime, execTime, NULL};
			execvp(arg[0], arg);
		}
		
	}
	return 0;
}