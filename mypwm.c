/* from: 
 http://man7.org/linux/man-pages/man2/timer_create.2.html */
 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#define SIG SIGRTMIN
#define CLKID CLOCK_REALTIME

//what to do after timer expires
void handler(int sig)
{
    printf("output HI\n");

}

void mypwm(int sleep_s, int nanosleep)
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
    its.it_interval.tv_nsec = 50000000;
    its.it_value.tv_sec = 3;
    its.it_value.tv_nsec = 0;
    
    if(timer_settime(timerid, 0, &its, NULL) == -1)
        perror("failed: timer settime\n");
    
    sleep(10); 
    

}
