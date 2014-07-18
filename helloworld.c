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
    mypwm();
//    timer(2, 10000000); 
   /* for (int i = 0; i < 5; i++) {
        printf("secs = %d\n", i);
        sleep(1);
    }*/

    printf("Bye, friends!\n");
    return 0;
}

