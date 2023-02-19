int main(int argc, char *argv[]){
printf("hello there from worker");
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