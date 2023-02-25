// Name: Gabrielle Hieken
// Class: 4760 Operating Systems
// Date: 2/15/2023

//To come back to: Reconfigure the help statement, and README, do I use the commented include?

#include <stdio.h>
#include <getopt.h> //Needed for optarg function
#include <stdlib.h> //EXIT_FAILURE
#include <unistd.h> //for pid_t and exec
#include <sys/types.h>
#include <time.h> // to create system time
#include <sys/shm.h> //Shared memory
#include "oss.h"

#define BILLION 1000000000L

//Create random second and nanosecond in bound of user input
int randomNumberGenerator(int limit)
{
    int sec;

    sec = (rand() % (limit)) + 1;

    return sec;
}

int main(int argc, char *argv[]){
	//number of total children to launch (n)
	int proc = 1;
    int totalChildren = 0;

	//how many children run at the same time (s)
	int simul = 1;

	//bound of time that a child process will be launched for (t)
	int timelimit= 2;
    int nanolimit = 32000;

    //variables for our system clock
    struct timespec start, currentTime, lastOutput;
    //start = NULL;
    double sec;
    double nano;

    //for wait call
    int stat;

    //child process ID
    pid_t childpid;

    //Parse through command line options
	char opt;
    while((opt = getopt(argc, argv, "hn:s:t:")) != -1 )
    {
        switch (opt)
        {
        //help message
        case 'h':
			printf("To run this project: \n\n");
            printf("run the command: ./oss -n num -s num -t num\n\n");
                    printf("\tnum = can be replaced with any number below 18 | -n, -s, and -t can be typed in any order\n");
                    printf("\t-n = number of total children to launch\n");
                    printf("\t-s = how many children run at the same time\n");
                    printf("\t-t = bound of time that a child process will be launched for\n\n");   
                    printf("If you leave out a '-n', '-s', or '-t' in the command line prompt it will defualt to the value 1 for all except  -t, which will be 2\n\n");
                    printf("Have fun :)\n\n");

                    exit(0);
            break;
        case 'n':
            proc = atoi(optarg);
			printf("proc,n: %i \n", proc);
            break;
        case 's':
            simul = atoi(optarg);
			printf("simul,s: %i \n", simul);
            break;
        case 't':
            timelimit = atoi(optarg);
			printf("timelimit,t: %i \n", timelimit);
            break;
        default:
            printf ("Invalid option %c \n", optopt);
            return (EXIT_FAILURE);
        }
    }

    //Create random second and nanosecond from user input
    srand(time(0));

    int seconds = randomNumberGenerator(timelimit);
    printf("This is your random number: %d \n\n", seconds);

    int nanoseconds = randomNumberGenerator(BILLION);
    printf("This is your nanosec: %d \n\n", nanoseconds);

    //Create shared memory, key
    const int sh_key = 3147550;

    int shm_id = shmget(sh_key, sizeof(struct PCB[20]), IPC_CREAT | 0666);
    if(shm_id <= 0) {
        fprintf(stderr,"ERROR: Failed to get shared memory, shared memory id = %i\n", shm_id);
        exit(1);
    }

    printf("Parent got sh_key: %i\n",sh_key);
    printf("Parent has id %i\n", shm_id);

    //attatch memory we allocated to our process and point pointer to it
    struct PCB *shm_ptr = (struct PCB*) (shmat(shm_id, NULL, 0));
    if (shm_ptr <= 0) {
        fprintf(stderr,"Shared memory attach failed\n");
        exit(1);
    }

     //start the simulated system clock
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }
    sleep(2);
    
    //currentTime simulated system clock
    if( clock_gettime( CLOCK_REALTIME, &currentTime) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }
    // lastOutput = currentTime;

    sec = (currentTime.tv_sec - start.tv_sec);
    nano = (double)( currentTime.tv_nsec - start.tv_nsec);

    printf("SysClockS: %lf SysClockNano: %lf \n", sec, nano);

    //Write the seconds and nanoseconds to memory for children to read
    //struct PCB writeToMem;
   //writeToMem.sec = sec;
    //writeToMem.nano = nano;


    //Loop to check for terminated children
// while (stillChildrenToLaunch) {
//     incrementClock();
//     Every half a second, output the process table to the screen
//     checkIfChildHasTerminated();
//     if (childHasTerminated, along the lines of this code -> int pid = waitpid(-1, &status, WNOHANG)) {
//         updatePCBOfTerminatedChild;
//         possiblyLaunchNewChild(obeying process limits)
//     }
// }

// while (totalChildren < proc){
//     if(start == NULL){
//         if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
//             perror( "clock gettime" );
//             return EXIT_FAILURE;
//         }
//         lastOutput = start;
//     }else{
//         if( clock_gettime( CLOCK_REALTIME, &currentTime) == -1 ) {
//             perror( "clock gettime" );
//             return EXIT_FAILURE;
//         }
//         sec = (currentTime.tv_sec - start.tv_sec);
//         nano = (double)( currentTime.tv_nsec - start.tv_nsec);
//         //printf("It has been %lf secs  %lf nanopseonds \n", sec, nano);
        
    //     double lastOutputTotal = lastOutput.tv_sec + lastOutput.tv_nsec/(BILLION);      // 5 + 6234234 = 5.623234
    //     double currentTimeTotal = currentTime.tv_sec + currentTime.tv_nsec/(BILLION);                        // 5 + 7234234 = 5.723234
    //     if((currentTimeTotal - lastOutputTotal) > 0.5){
    //         ouytputprocesstable();
    //         lastOutput = currentTime;
    //     }
    // }

//     //fork child processes
//     childpid = fork();
//     if (childpid == -1) {
//         perror("Failed to fork");
//         return 1;
//     }
//     totalChildren++;
    
//     //send shared memory key to worker for children to use
//     if (childpid == 0){ 
//         char sh_key_string[50];
//         snprintf(sh_key_string, sizeof(sh_key_string), "%i", sh_key);

//         //exec function to send children to worker
//         char *args[] = {"worker", sh_key_string, NULL};
//         execvp("./worker", args);
//         return 1;
//     }
//     else {
//         processTable[totalChildren].nano = currentTime.tv_nsec;
//         processTable[totalChildren].sec = currentTime.tv_sec;
//         processTable[totalChildren].pid = childpid;
//         processTable[totalChildren].occupied = true;
//         *shm_ptr = processTable;
//     }
// }

    // printf("memSec: %lf memNano: %lf \n", writeToMem.sec, writeToMem.nano);
    // *shm_ptr = writeToMem;
    
    // writeToMem = *shm_ptr;
    // printf("Wrote to memory: memSec: %lf memNano: %lf \n", writeToMem.sec, writeToMem.nano);
    
    struct PCB* processTable[20];

    int i;
    for (i = 1; i <= proc; i++){

        printf("i is %i", i);
        processTable[i]->nano = nano;
        processTable[i]->sec = sec;
        //processTable[i].pid = childpid;
        processTable[i]->occupied = 1;
        printf("memSec: %lf memNano: %lf \n", processTable[i]->sec, processTable[i]->nano);
        *shm_ptr = *processTable;

        processTable[i]->nano = 5;
        processTable[i]->sec = 77;

        printf("Fucked up memory memSec: %lf memNano: %lf \n", processTable[i]->sec, processTable[i]->nano);

        *processTable = *shm_ptr;
        printf("Wrote to memory:: memSec: %lf memNano: %lf \n", processTable[i]->sec, processTable[i]->nano);


        //fork child processes
        childpid = fork();
        if (childpid == -1) {
            perror("Failed to fork");
            return 1;
        }
        
        //send shared memory key to worker for children to use
        if (childpid == 0){ 
            char sh_key_string[50];
            snprintf(sh_key_string, sizeof(sh_key_string), "%i", sh_key);

            //exec function to send children to worker
            char *args[] = {"worker", sh_key_string, NULL};
            execvp("./worker", args);
            return 1;
        }
        else {
            //wait for the process to finish after running the given simul int simultaneously before starting another process
            if(i >= simul){
                wait(&stat);
            }
        }
    }
    printf("deleting memory");
    shmdt( shm_ptr ); // Detach from the shared memory segment
    shmctl( shm_id, IPC_RMID, NULL ); // Free shared memory segment shm_id

///////////////////////////////////////////////////////////



return 0;
}