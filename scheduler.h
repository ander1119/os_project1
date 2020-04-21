#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "process.h"

#define UNINITIAL 0
#define WAIT 1
#define DONED 2

#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4

void schedule(struct Process processList[], int processNum, int schedulingPolicy);

int nextProcess(struct Process processList[], int processNum, int schedulingPolicy);

#endif