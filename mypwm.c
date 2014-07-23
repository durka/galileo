/* from: 
 http://man7.org/linux/man-pages/man2/timer_create.2.html */
 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <helloworld.h>
#define SIG SIGRTMIN
#define CLKID CLOCK_REALTIME


//global varialbes
volatile int exit_sig = 0;
int count = 0;
int high = 0;
struct itimerspec its; //timing information
timer_t timerid;
time_t highT = 2800000; //if 50% dc, 2800000ns of high time outputs ~180 Hz sq wave

//val is initial value of timer, int (interval), is every subsequent value 
//if int is 0, timer expires once only
void set_time(time_t val_s, time_t val_ns, time_t int_s, time_t int_ns)
{
    its.it_value.tv_sec = val_s;
    its.it_value.tv_nsec = val_ns;
    its.it_interval.tv_sec = int_s;
    its.it_interval.tv_nsec = int_ns; 
} 

//what to do after timer expires
void handler(int sig, siginfo_t *si, void *uc)
{
//will exit after about 5s for freq of ard 180 Hz
    if (count > 2000) {
        exit_sig = 1;
        return;
    }
    else { 
        count++; 
    }
    //arm timer again
    //same timing interval for hi and lo for 50% duty cycle
    set_time(0, highT, 0, 0);

    if(timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("failed: ");
        exit_sig = 1;
    }
    //oscillate between high and low
    if (high) {
        echo("/sys/class/gpio/gpio27/value", "1");
        high = 0;
    }
    else {
        echo("/sys/class/gpio/gpio27/value", "0");
        high = 1;
    }

}

void mypwm()
{
    //init timer
    struct sigevent sev;
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;

    //init handler for signal
    if (sigaction(SIG, &sa, NULL) == -1)
        perror("sigaction: ");
  
    //create timer    
    if (timer_create(CLKID, &sev, &timerid) == -1)
        perror("timer create: \n");


    set_time(0, highT, 0, 0);

    if(timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("failed: ");
        exit_sig = 1;
    }

    //while loop to keep going
    while (1) {
        if (exit_sig)
            break;
         
    }
}
