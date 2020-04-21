#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include "process.h"

#define UNINITIAL 0
#define WAIT 1
#define DONED 2

#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4

void schedule(struct Process *processList, int processNum, int schedulingPolicy);

int nextProcess(struct Process *processList, int processNum, int schedulingPolicy);