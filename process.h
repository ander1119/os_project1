#ifndef _PROCESS_H
#define _PROCESS_H

#define schedulerCPU 0
#define processCPU 1
#define PRINTK 548
#define GETTIME 549

struct Process{
	char processName[32];
	int readyTime;
	int execTime;
	int leftExecTime;
	int pid;
	int status;
};

void unitTime();

void assignCPU(int pid, int coreIndex);

int initProcess(int execTime);

void setHighPriority(int pid);

void setLowPriority(int pid);

#endif