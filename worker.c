#include <stdio.h>
#include <sys/shm.h> //Shared memory
#include "oss.h"

int main(int argc, char *argv[]){
    printf("hello there from worker \n");
    int termTimeS;
    int termTimeNano;
    int sysClockS;
    int sysClockNano;
    int secPassed;

    int sh_key = atoi(argv[1]);
    int sec = atoi(argv[2]);
    int nano = atoi(argv[3]);

    printf("Child got sh_key: %i\n", sh_key);
    printf("worker got random second: %i\n", sec);
    printf("worker got random nanosecond: %i\n", nano);

    int shm_id = shmget(sh_key, sizeof(struct PCB), 0666);
    if(shm_id <= 0) {
        fprintf(stderr,"CHILD ERROR: Failed to get shared memory, shared memory id = %i\n", shm_id);
        exit(1);
    }

    printf("child opened id %i\n", shm_id);

    //attatch memory we allocated to our process and point pointer to it 
    struct PCB *shm_ptr = (struct PCB*) (shmat(shm_id, 0, 0));
    if (shm_ptr <= 0) {
        fprintf(stderr,"Child Shared memory attach failed\n");
        exit(1);
    }
    
    struct PCB readFromMem;
    readFromMem = *shm_ptr;

    termTimeS = readFromMem.sec + sec;
    termTimeNano = readFromMem.nano + nano;

    sysClockS = readFromMem.sec;
    sysClockNano = readFromMem.nano;

    secPassed = sec - readFromMem.sec;

    printf("Child: Read Value - memSec: %lf memNano: %lf \n", readFromMem.sec, readFromMem.nano);
    printf("when it will terminate sec + memsec: %i", readFromMem.sec);

    printf("\n\nsysClockS: %ld\n", sysClockS);
    printf("sysClockNano: %ld\n", sysClockNano);
    printf("termTimeS: %ld\n", termTimeS);
    printf("termTimeNano: %ld\n", termTimeNano);
    printf("secPassed: %ld\n\n", secPassed);


    //printf("WORKER PID: %ld PPID: %ld SysClockS: %i SysclockNano: %i TermTimeS: %i TermTimeNano: %i\n--Just Starting",(long)getpid(), (long)getppid(), sysClockS, sysClockNano, termTimeS, termTimeNano);

    // while(1){
    //     if(termTimeS <= sysClockS){
    //         if (termTimeNano <= sysClockNano){
    //             break;
    //         }
    //     }
    //     printf("WORKER PID: %ld PPID: %ld SysClockS: %i SysclockNano: %i TermTimeS: %i TermTimeNano: %i\n --%i seconds has passed",(long)getpid(), (long)getppid(), sysClockS, sysClockNano, termTimeS, termTimeNano, secPassed);
    // }

    //printf("WORKER PID: %ld PPID: %ld SysClockS: %i SysclockNano: %i TermTimeS: %i TermTimeNano: %i\n --Terminating",(long)getpid(), (long)getppid(), sysClockS, sysClockNano, termTimeS, termTimeNano);

    return 0;
}