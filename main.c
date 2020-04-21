#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "process.h"
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
	
	if(strcmp(buf, "FIFO") == 0) schedulingPolicy = FIFO;
	else if(strcmp(buf, "RR") == 0) schedulingPolicy = RR;
	else if(strcmp(buf, "SJF") == 0) schedulingPolicy = SJF;
	else if(strcmp(buf, "PSJF") == 0) schedulingPolicy = PSJF;

	for(int i=0 ; i<processNum ; i++)
		scanf("%s%d%d", processList[i].processName, &processList[i].readyTime, &processList[i].execTime);
	
	qsort(processList, processNum, sizeof(struct Process), cmpReadytime);

	fprintf(stderr, "After sort:\n");
	for(int i=0 ; i<processNum ; i++)
		fprintf(stderr, "%s %d %d\n", processList[i].processName, processList[i].readyTime, processList[i].execTime);

	schedule(processList, processNum, schedulingPolicy);
	
	return 0;
}