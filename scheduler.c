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
				processList[i].leftExecTime = processList[i].execTime;
				setLowPriority(processList[i].pid);
			}
			else if(processList[i].readyTime > currentTime){
				break;
			}
		}

		if(runningIndex != -1 && processList[runningIndex].leftExecTime <= 0){ //the running process ends
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

		unitTime();
		if(runningIndex != -1){
			processList[runningIndex].leftExecTime--;

#ifdef DEBUG
			if(processList[runningIndex].leftExecTime % 100 == 0){
				fprintf(stderr, "runningIndex = %d, process pid = %d, left %d execution sec\n", runningIndex, processList[runningIndex].pid, processList[runningIndex].leftExecTime);
			}
#endif

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
		if(runningIndex == -1){
			for(int i=0 ; i<processNum ; i++){
				if(processList[i].status == WAIT)
					return i;
			}
			return -1;
		}
		else if(processList[runningIndex].execTime != processList[runningIndex].leftExecTime && processList[runningIndex].execTime - processList[runningIndex].leftExecTime % 500 == 0){
			int nextIndex = (runningIndex + 1) % processNum;
			while(1){
				if(processList[nextIndex].status == WAIT)
					return nextIndex;
				nextIndex = (nextIndex + 1) % processNum;
			}
		}
	}
	else if(schedulingPolicy == SJF){
		if(runningIndex != -1){
			return runningIndex;
		}
		else{
			int shortestJobIndex = -1;
			for(int i=0 ; i<processNum ; i++){
				if(shortestJobIndex == -1 && processList[i].status == WAIT)
					shortestJobIndex = i;
				else if(processList[i].status == WAIT && processList[i].execTime < processList[shortestJobIndex].execTime){
					shortestJobIndex = i;
				}
			}
			return shortestJobIndex;
		}	
	}
	else if(schedulingPolicy == PSJF){	
		int shortestJobIndex = -1;
		for(int i=0 ; i<processNum ; i++){
			if(shortestJobIndex == -1 && processList[i].status == WAIT)
				shortestJobIndex = i;
			else if(processList[i].status == WAIT && processList[i].leftExecTime < processList[shortestJobIndex].leftExecTime){
				shortestJobIndex = i;
			}
		}
		return shortestJobIndex;
	}
	return -1;
}
