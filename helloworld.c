#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "pwm.h"
#include "i2c.h"
#include "analogin.h"
#include "timer.h"
#include "UtilTime.h"

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
    echo("/sys/class/gpio/export", "30");
    echo("/sys/class/gpio/gpio30/direction", "out");
    echo("/sys/class/gpio/gpio30/value", "1"); 

    echo("/sys/class/gpio/export", "18");
    echo("/sys/class/gpio/gpio18/direction", "out");
    echo("/sys/class/gpio/gpio18/value", "1"); 
    echo("/sys/class/gpio/gpio18/drive", "strong"); 

    echo("/sys/class/pwm/pwmchip0/export", "3");
    echo("/sys/class/pwm/pwmchip0/pwm3/enable", "1");
//    echo("/sys/class/pwm/pwmchip0/pwm3/period", "250000"); 
 //   echo("/sys/class/pwm/pwmchip0/pwm3/duty_cycle", "125000"); 
    
}

void deactivate_pins()
{
    echo("/sys/class/pwm/pwmchip0/pwm3/duty_cycle", "0"); 
    echo("/sys/class/pwm/pwmchip0/pwm3/period", "1"); 
    echo("/sys/class/pwm/pwmchip0/pwm3/enable", "0");
    echo("/sys/class/pwm/pwmchip0/unexport", "3");

    echo("/sys/class/gpio/unexport", "30");

    echo("/sys/class/gpio/gpio18/value", "0"); 
    echo("/sys/class/gpio/unexport", "18");

}

int main(int argc, char const* argv[])
{
    printf("Hello, world!\n");
    deactivate_pins();
    init();
    pwm(); 


    printf("Bye, friends!\n");
    return 0;
}

