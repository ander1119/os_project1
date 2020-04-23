#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

void unitTime(){
	volatile unsigned long i; 
	for(i=0;i<1000000UL;i++);
}

void assignCPU(int pid, int coreIndex){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(coreIndex, &mask);	//assign specific core into mask

	if (sched_setaffinity(0, sizeof(mask), &mask) == -1){
		fprintf(stderr, "can't set CPU affinity/n");
		exit(0);
	}

	return;
}

void setHighPriority(int pid, int priority){
	struct sched_param param;
	param.sched_priority = priority;
	if(sched_setscheduler(pid, SCHED_RR, &param) < 0){
		fprintf(stderr, "can't set pid = %d high priority\n", pid);
		exit(0);
	}
}

void setLowPriority(int pid){
	struct sched_param param;
	param.sched_priority = 10;
	if(sched_setscheduler(pid, SCHED_RR, &param) < 0){
		fprintf(stderr, "can't set pid = %d low priority\n", pid);
		exit(0);
	}
}

int initProcess(int execTime){
	int pid;
	if((pid = fork()) == 0){
		unsigned long startSec, startNSec, finishSec, finishNSec;
		char printkBuffer[40];
		syscall(GETTIME, &startSec, &startNSec);
		
		for(int i=0 ; i<execTime ; i++){

#ifdef DEBUG
			if(i % 100 == 0){
				fprintf(stderr, "pid = %d run %d unit time on %d cpu\n", getpid(), i, sched_getcpu());
			}
#endif

			unitTime();
		}
		syscall(GETTIME, &finishSec, &finishNSec);
		sprintf(printkBuffer, "[Project1] %d %9lu.%09lu %9lu.%09lu", getpid(), startSec, startNSec, finishSec, finishNSec);
		syscall(PRINTK, printkBuffer, 100);
		fprintf(stderr, "pid %d finish\n", getpid());
		exit(0);
	}
	else{
		assignCPU(pid, processCPU);
		return pid;
	}
}