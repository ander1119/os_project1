#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"
#define maxChild 100

int cmpReadytime(const void *a, const void *b){
	return ((struct Process *)a)->readyTime < ((struct Process *)b)->readyTime;
}

int main(){
	char buf[10];
	int processNum;
	int schedulingPolicy;
	struct Process processList[maxChild];
	scanf("%s%d", buf, &processNum);
	
	if(strcmp(buf, "FIFO") == 0) schedulingPolicy = 1;
	else if(strcmp(buf, "RR") == 0) schedulingPolicy = 2;
	else if(strcmp(buf, "SJF") == 0) schedulingPolicy = 3;
	else if(strcmp(buf, "PSJF") == 0) schedulingPolicy = 4;

	for(int i=0 ; i<processNum ; i++)
		scanf("%s%d%d", processList[i].processName, &processList[i].readyTime, &processList[i].execTime);
	
	qsort(processList, processNum, sizeof(struct Process), cmpReadytime);

	schedule(processList, processNum, schedulingPolicy);
	
	return 0;
}