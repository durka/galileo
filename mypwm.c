/* from: 
 http://man7.org/linux/man-pages/man2/timer_create.2.html */
 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#define SIG SIGRTMIN
#define CLKID CLOCK_REALTIME

//global varialbes
int exit_sig = 0;
int count = 0;
int out = 0;

//what to do after timer expires
void handler(int sig)
{
    printf("handle sig\n");
    if (out) {
        printf("HIGH\n");
        out = 0;
    }
    else {
        printf("LOW\n");
        out = 1;
    }
//increase count or activate exit signal
    if (count > 9)
        exit_sig = 1;
    else 
        count++; 

}

void mypwm()
{

    timer_t timerid;
    struct sigevent sev;
    struct sigaction sa;
    struct itimerspec its;
    sa.sa_flags = SA_RESETHAND;
    sa.sa_handler = handler;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;

    //init handler for signal
    if (sigaction(SIG, &sa, NULL) == -1)
        perror("sigaction");
  
    //create timer    
    if(timer_create(CLKID, &sev, &timerid) == -1)
        printf("failed: timer create\n");

    printf("timerid: %d\n", timerid);
    printf("sig id: %d\n", SIG);
    
    //start timer
    //set timing intervals
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0; 
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
  /*  
    if(timer_settime(timerid, 0, &its, NULL) == -1)
        perror("failed: timer settime\n");
    sleep(2);
*/
    //while loop to keep going
    while (!exit_sig) {
        if(timer_settime(timerid, 0, &its, NULL) == -1) {
            perror("failed: timer settime\n");
            exit_sig = 1;
        }
    }
}
