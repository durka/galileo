#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "pwm.h"
#include "i2c.h"
#include "analogin.h"
#include "timer.h"
#include "UtilTime.h"
#include "mypwm.h"

int echo(char filename[], char value[])
{
    int write_true;
    int fd = open(filename, O_WRONLY);
    if (fd == -1)
    {
        printf("\topen: %s\n", strerror(errno));
    }
    int n = strlen(value);
    int n_wrote  = write(fd, value, n);
    printf("wrote %d/%d of %s to %s\n", n_wrote, strlen(value), value, filename);
    if (n_wrote == -1)
    {
        printf("\twrite: %s\n", strerror(errno));
    }
    close(fd);
    return n_wrote;
    
}

void init()
{
    //gpio27 is digital pin 7
/*
    echo("/sys/class/gpio/export", "27");
    echo("/sys/class/gpio/gpio27/direction", "out");
    echo("/sys/class/gpio/gpio27/drive", "strong"); 
    echo("/sys/class/gpio/gpio27/value", "0"); 
*/    
    
}

void deactivate_pins()
{
    echo("/sys/class/gpio/gpio27/value", "0"); 
    echo("/sys/class/gpio/unexport", "27");

}

int main(int argc, char const* argv[])
{
    printf("Hello, world!\n");
    //sleep for 12500 ns (half period of 40 kHz)
   /* struct timespec interval;
    struct timespec remaining;
    interval.tv_sec = 0;
    interval.tv_nsec = 12500;
*/
//    timer(1, 100);
    mypwm(1, 100);
 /*   for (int i = 0; i < 80000; i++) {
        echo("/sys/class/gpio/gpio27/value", "1"); 
        nanosleep(&interval, &remaining);
        echo("/sys/class/gpio/gpio27/value", "0"); 
        nanosleep(&interval, &remaining);
    }
   */ 

    printf("Bye, friends!\n");
    return 0;
}

