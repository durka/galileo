/*set a pwm output*/
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "i2c.h"
#include "dsp.h"
#include "i2c-dev.h"
//global varialbes
int cypress;
volatile int exit_sig = 0;
int count = 0;
int high = 0;
int T_count = 0;
struct itimerspec its; //timing information
timer_t timerid;
time_t highT = 1000000; //

//macros to write to cypress chip
#define WCYPRESS(...) {\
    unsigned char buf[] = { __VA_ARGS__ };\
    i2c_writebytes(cypress, buf, sizeof(buf)); }

//timing variables
#define SIG SIGRTMIN
#define CLKID CLOCK_REALTIME

//get clk freq based on input to cypress registers
double getfreq(int clkno, int T, int div)
{
    double clkf;
    double scale;
    double f_actual;

    switch (clkno) {
        case 0:
            clkf = 32000;
            scale = 1.23;
            break;
        case 1:
            clkf = 24000000;
            scale = 0.975;
            break;  
        case 2:
            clkf = 1500000;
            scale = 0.975;
            break;
        case 3:
            clkf = 93750;
            scale = 0.975;
            break;
        case 4:
            clkf = 93750.0/div;         
            scale = 0.975; 
            break;
        default:
            clkf = clkno;
            scale = 0.997;
            break;
    } 

    f_actual = clkf/(T*scale); 
    return f_actual;
}

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
//change duty cycle
    if (high){
        high = 0;
        WCYPRESS(0x2B, 40);
    }
    else {
        high = 1;
        WCYPRESS(0x2B, 160);
    }
/*
//reset timer
    set_time(0, highT, 0, 0);

    if(timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("failed: ");
        exit_sig = 1;
    }
       */ 
    
}
//write to the cypress pwm
int pwm()
{
    //establish connection with cypress
    cypress = i2c_openadapter(0);
    i2c_setslave(cypress, 0x20) ;
    printf("Opened cypress: %d\n", cypress);

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

    set_time(0, highT, 0, highT);
    
    //PWM select, clockid, period, pulse width, f dividier
    //format:reg1, val-of-reg1, val-of-reg2, val-of-reg3 ...
    //40 kHz pwm
    WCYPRESS(0x28, 2, 1, 3, 2);
    WCYPRESS(0x28, 3, 5, 205, 102);

    /*if(timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("failed: ");
        exit_sig = 1;
    }*/
    while (1) { handler(0, NULL, NULL); }

   // int clkid1 = 1; int T1 = 3; int T2 = 205;
  //  printf("clk freq: %f Hz\n", getfreq(getfreq(clkid1, T1, 1), T2, 1)); 

    while (1) {
        if (exit_sig)
            break;
    }

    return 0;
}


