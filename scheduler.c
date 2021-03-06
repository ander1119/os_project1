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

struct Queue{
	int array[Qlength];
	int start;
	int end;
};

struct Queue Q;

int deQueue(){
	if(Q.start == Q.end) return -1;
	int tmp = Q.array[Q.start];
	Q.start = (Q.start + 1) % Qlength;
	return tmp;
}

void enQueue(int idx){
	Q.array[Q.end] = idx;
	Q.end = (Q.end + 1) % Qlength;
}

void schedule(struct Process processList[], int processNum, int schedulingPolicy){
	for(int i=0 ; i<processNum ; i++) processList[i].status = UNINITIAL; //the process isn't initial yet

	setHighPriority(getpid(), schedulerCPU);	//set scheduler priority higher than others

	Q.start = 0;
	Q.end = 0;

	while(1){
		if(finishNum == processNum) 	//all processes end
			break;

		for(int i=0 ; i<processNum ; i++){	//initial process when currentTime meets readyTime;
			if(processList[i].readyTime == currentTime){
				processList[i].pid = initProcess(processList[i].execTime);
				processList[i].status = WAIT;
				processList[i].leftExecTime = processList[i].execTime;
				setLowPriority(processList[i].pid, processCPU);
				enQueue(i);
			}
			else if(processList[i].readyTime > currentTime){
				break;
			}
		}

		if(runningIndex != -1 && processList[runningIndex].leftExecTime <= 0){ //the running process ends
			kill(processList[runningIndex].pid, SIGUSR1);
			waitpid(processList[runningIndex].pid, NULL, 0);
			fprintf(stdout, "%s %d\n", processList[runningIndex].processName, processList[runningIndex].pid);
			fflush(stdout);
			processList[runningIndex].status = DONED;
			runningIndex = -1;
			finishNum++;
		}

		int nextIndex = nextProcess(processList, processNum, schedulingPolicy);
		if(nextIndex != -1 && runningIndex != nextIndex){
			if(processList[nextIndex].execTime == processList[nextIndex].leftExecTime)
				kill(processList[nextIndex].pid, SIGUSR2);
			setHighPriority(processList[nextIndex].pid, processCPU);
			if(runningIndex != -1)
				setLowPriority(processList[runningIndex].pid, processCPU);
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
			return deQueue();
		}
		else if(processList[runningIndex].execTime != processList[runningIndex].leftExecTime && (processList[runningIndex].execTime - processList[runningIndex].leftExecTime) % 500 == 0){
			enQueue(runningIndex);
			return deQueue();
		}
		else{
			return runningIndex;
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
