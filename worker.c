#include <stdio.h>
#include <sys/shm.h> //Shared memory
#include "oss.h"

int main(int argc, char *argv[]){
    printf("hello there from worker \n");
    int sh_key = atoi(argv[1]);
    printf("Child got sh_key: %i", sh_key);


    int shm_id = shmget(sh_key, sizeof(struct PCB), 0666);
    if(shm_id <= 0) {
        fprintf(stderr,"CHILD ERROR: Failed to get shared memory, shared memory id = %i\n", shm_id);
        exit(1);
    }

    //attatch memory we allocated to our process and point pointer to it
    struct PCB *shm_ptr = (struct PCB*) (shmat(shm_id, 0, 0));
    if (shm_ptr <= 0) {
        fprintf(stderr,"Child Shared memory attach failed\n");
        exit(1);
    }
    struct PCB readFromMem;
    readFromMem = *shm_ptr;

    printf("Child: Read Value - memSec: %lf memNano: %lf \n", readFromMem.sec, readFromMem.nano);

    return 0;
//     WORKER PID:6577 PPID:6576 SysClockS: 5 SysclockNano: 1000 TermTimeS: 11 TermTimeNano: 500100
// --Just Starting

// WORKER PID:6577 PPID:6576 SysClockS: 6 SysclockNano: 45000000 TermTimeS: 11 TermTimeNano: 500
// --1 seconds have passed since starting

// WORKER PID:6577 PPID:6576 SysClockS: 7 SysclockNano: 500000 TermTimeS: 11 TermTimeNano: 50010
// --2 seconds have passed since starting

// WORKER PID:6577 PPID:6576 SysClockS: 11 SysclockNano: 700000 TermTimeS: 11 TermTimeNano: 5001
// --Terminating
}