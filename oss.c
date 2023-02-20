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

#include <stdint.h> 

#define BILLION 1000000000L

//#include <sys/wait.h>

struct PCB {
int occupied; // either true or false
pid_t pid; // process id of this child
int startSeconds; // time when it was forked
int startNano; // time when it was forked
};
struct PCB processTable[20];

//Create random second and nanosecond from user input
int randomNumberGenerator(int limit)
{
    int sec;

    sec = (rand() % (limit)) + 1;

    return sec;
}

int main(int argc, char *argv[]){
	//number of total children to launch (n)
	int proc = 1;
	//how many children run at the same time (s)
	int simul = 1;
	//bound of time that a child process will be launched for (t)
	int timelimit= 2;
    int nanolimit = 32000;

    //My cock 
    // struct PCB start, end;
    // uint64_t nanosecond;
    // time_t second;

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

    int nanoseconds = randomNumberGenerator(nanolimit);
    printf("This is your nanosec: %d \n\n", nanoseconds);


/////////////////////////////////////////////////////////
    //Create shared memory
    const int sh_key = 3147550;

    int shm_id = shmget(sh_key, sizeof(int)*10, IPC_CREAT | 0666);
    if(shm_id <= 0) {
        fprintf(stderr,"ERROR: Failed to get shared memory, shared memory id = %i\n", shm_id);
        exit(1);
    }

    //attatch memory we allocated to our process and point pointer to it
    int *shm_ptr = (int*) (shmat(shm_id, 0, 0));
    if (shm_ptr <= 0) {
        fprintf(stderr,"Shared memory attach failed\n");
        exit(1);
    }

    struct timespec start, stop;
    double sec;
    double nano;

    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    sleep(2);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    sec = ( stop.tv_sec - start.tv_sec );
    nano = (double)( stop.tv_nsec - start.tv_nsec);

    printf("SysClockS: %lf SysClockNano: %lf \n", sec, nano);

    double together = sec + nano/(double)BILLION;
    printf("Together (in seconds): %lf \n", together);

    struct SecStruct{
        double sec;
        double nano;
    };
    struct SecStruct writeToMem;
    writeToMem.sec = sec;
    writeToMem.nano = nano;

    printf("memSec: %lf memNano: %lf \n", writeToMem.sec, writeToMem.nano);




    int i; 
    for(i = 0; i < 5; i++){
        *shm_ptr = 10 + i;
        printf("Parent: Written Val.: %d\n", *shm_ptr);
    }


    //fork child processes
    childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    
    //send shared memory key to use in worker
    if (childpid == 0){ 
        char sh_key_string[50];
        snprintf(sh_key_string, sizeof(sh_key_string), "%i", sh_key);

        //exec function to send children to worker
        char *args[] = {"worker", sh_key_string, NULL};
        execvp("./worker", args);
        return 1;
        printf("Brydens a nice person");
    }

    int stat;
    wait(&stat);    //Wait for child process to finish before deleting the memory

    shmdt( shm_ptr ); // Detach from the shared memory segment
    shmctl( shm_id, IPC_RMID, NULL ); // Free shared memory segment shm_id

///////////////////////////////////////////////////////////

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

return 0;
}