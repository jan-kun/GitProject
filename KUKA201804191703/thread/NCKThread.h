#ifndef NCKTHREAD_H
#define NCKTHREAD_H

#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

struct period_info {
        struct timespec next_period;
        long period_ns;
};

extern void CreatThread(void);

#endif
