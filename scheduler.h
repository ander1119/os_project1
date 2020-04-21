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

void schedule(struct Process processList[], int processNum, int schedulingPolicy);

int nextProcess(struct Process processList[], int processNum, int schedulingPolicy);