#include <stdio.h>
#include <unistd.h> //for pid_t and exec

struct PCB {
int occupied; // either true or false
pid_t pid; // process id of this child
int sec; // time when it was forked
int nano; // time when it was forked
};
struct PCB processTable[20];