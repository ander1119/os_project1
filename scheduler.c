#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"
#include "process.h"

static int currentTime = 0; 	//how long has it been

static int runningIndex = -1; 	//which process is running

static int finishNum = 0; 		//how many process has doned

void schedule(struct Process *processList, int processNum, int schedulingPolicy){
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
			else if(processList[i].readyTime > currentTime)
				break;
		}

		if(runningIndex != -1 && processList[runningIndex].execTime == 0){ //the running process ends
			wait(processList[runningIndex].pid);
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

		unitTime();
		if(runningIndex != -1)
			processList[runningIndex].execTime--;
		currentTime++;
	}	
}

int nextProcess(struct Process *processList, int processNum, int schedulingPolicy){
	if(schedulingPolicy == 1){		//FIFO
		for(int i=0 ; i<processNum ; i++){
			if(processList[i].status == UNINITIAL)
				return i;
		}
	}
	else if(schedulingPolicy == 2){	//RR
		return -1;
	}
	else if(schedulingPolicy == 3){	//SJF
		return -1;
	}
	else if(schedulingPolicy == 4){	//PSJF
		return -1;
	}
}
