#include <stdio.h>
#include <unistd.h> //for pid_t and exec


struct PCB {
    int occupied; // either true or false
    pid_t pid; // process id of this child
    double sec; // time when it was forked
    double nano; // time when it was forked
};
//struct PCB processTable[20];
//OSS PID:6576 SysClockS: 7 SysclockNano: 500000
//Process Table:
//Entry Occupied PID   StartS   StartN
//0     1        6577  5        500000