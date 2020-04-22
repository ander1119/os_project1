#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include "scheduler.h"
#include "process.h"

static int currentTime = 0; 	//how long has it been

static int runningIndex = -1; 	//which process is running

static int finishNum = 0; 		//how many process has doned

void schedule(struct Process processList[], int processNum, int schedulingPolicy){
	for(int i=0 ; i<processNum ; i++) processList[i].status = UNINITIAL; //the process isn't initial yet

	assignCPU(getpid(), schedulerCPU);	//assign scheduler unique CPU

	setHighPriority(getpid());			//set scheduler priority higher than others

	while(1){
		if(finishNum == processNum) 	//all processes end
			break;

		for(int i=0 ; i<processNum ; i++){	//initial process when currentTime meets readyTime;
			if(processList[i].readyTime == currentTime){
				processList[i].pid = initProcess(processList[i].execTime);
				processList[i].status = WAIT;
				setLowPriority(processList[i].pid);
			}
			else if(processList[i].readyTime > currentTime){
				break;
			}
		}

		if(runningIndex != -1 && processList[runningIndex].execTime == 0){ //the running process ends
			waitpid(processList[runningIndex].pid, NULL, 0);
			printf("%s %d\n", processList[runningIndex].processName, processList[runningIndex].pid);
			processList[runningIndex].status = DONED;
			runningIndex = -1;
			finishNum++;
		}

		int nextIndex = nextProcess(processList, processNum, schedulingPolicy);
		if(nextIndex != -1 && runningIndex != nextIndex){
			setHighPriority(processList[nextIndex].pid);
			if(runningIndex != -1)
				setLowPriority(processList[runningIndex].pid);
			runningIndex = nextIndex;
		}
/*
#ifdef DEBUG
		fprintf(stderr, "runningIndex = %d\n", runningIndex);
#endif
*/
		unitTime();
		if(runningIndex != -1){
			processList[runningIndex].execTime--;
/*
#ifdef DEBUG
			if(processList[runningIndex].execTime % 100 == 0){
				fprintf(stderr, "process pid = %d, left %d execution sec\n", processList[runningIndex].pid, processList[runningIndex].execTime);
			}
#endif
*/
		}
		currentTime++;
	}	
}

int nextProcess(struct Process processList[], int processNum, int schedulingPolicy){
	if(schedulingPolicy == FIFO){
		if(runningIndex != -1)
			return runningIndex;		
		for(int i=0 ; i<processNum ; i++){
			if(processList[i].status == WAIT)
				return i;
		}
	}
	else if(schedulingPolicy == RR){
		return -1;
	}
	else if(schedulingPolicy == SJF){	
		return -1;
	}
	else if(schedulingPolicy == PSJF){	
		return -1;
	}
	return -1;
}
