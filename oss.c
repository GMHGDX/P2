// Name: Gabrielle Hieken
// Class: 4760 Operating Systems
// Date: 2/15/2023

//To come back to: Reconfigure the help statement, and README, do I use the commented include?

#include <stdio.h>
#include <getopt.h> //Needed for optarg function
#include <stdlib.h> //EXIT_FAILURE
#include <unistd.h> //for pid_t and exec
#include <sys/types.h>
//#include <sys/wait.h>

int main(int argc, char *argv[]){
	//number of total children to launch (n)
	int proc = 1;
	//how many children run at the same time (s)
	int simul = 1;
	//bound of time that a child process will be launched for (t)
	int timelimit= 1;

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
                    printf("If you leave out a '-n', '-s', or '-t' in the command line prompt it will defualt to the value 1\n\n");
                    printf("Have fun :)\n\n");

                    exit(0);
            break;
        case 'n':
            proc = atoi(optarg);
			//printf("proc,n: %i \n", proc);
            break;
        case 's':
            simul = atoi(optarg);
			//printf("simul,s: %i \n", simul);
            break;
        case 't':
            timelimit = atoi(optarg);
			//printf("timelimit,t: %i \n", timelimit);
            break;
        default:
            printf ("Invalid option %c \n", optopt);
            return (EXIT_FAILURE);
        }
    }