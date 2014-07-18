/*set a pwm output*/
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "i2c.h"
#include "helloworld.h"

//macros to control pwm output
#define CYPRESS(...) { unsigned char buf[] = { __VA_ARGS__ }; CYPRESS_(buf); }
#define CYPRESS_(b) ( send( cypress, 0x20, sizeof (b), (b) ) )
#define READ_CYPRESS(...) { unsigned char buf[] = { __VA_ARGS__ }; READ_CYPRESS_(buf); }
#define READ_CYPRESS_(b) ( read( cypress, 0x20, sizeof (b), (b) ) )

void send(int adapter, unsigned char address, int len, char buf[])
{
    i2c_setslave(adapter, address);
    i2c_writebytes(adapter, buf, len);
}

void read(int adapter, unsigned char address, int len, char buf[])
{
    i2c_setslave(adapter, address);
    i2c_readbytes(adapter, buf, len);
}
void init_pwm() 
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
    
}

void deactivate_pwm() 
{
    echo("/sys/class/pwm/pwmchip0/pwm3/duty_cycle", "0");
    echo("/sys/class/pwm/pwmchip0/pwm3/period", "1");
    echo("/sys/class/pwm/pwmchip0/pwm3/enable", "0");
    echo("/sys/class/pwm/pwmchip0/unexport", "3");
    
    echo("/sys/class/gpio/unexport", "30");

    echo("/sys/class/gpio/gpio18/value", "0");
    echo("/sys/class/gpio/unexport", "18");
}    

int pwm()
{
    int cypress = i2c_openadapter(0);
    printf("Opened cypress: %d\n", cypress);
    
    //PWM select, clock source, period, pulse width, f dividier
    //format:reg1, val-of-reg1, val-of-reg2, val-of-reg3 ...
    /*for (int i = 1; i < 5; i++) {
        CYPRESS(0x28, 2, 1, 3, 2);
        CYPRESS(0x28, 3, 5, 205, 180);

        usleep(31);

        CYPRESS(0x28, 2, 1, 3, 2);
        CYPRESS(0x28, 3, 5, 205, 10);
        
        usleep(31);
    }*
*
    for (int i = 0; i < 10; i++) {  
        CYPRESS(0x28, 3, 4, 150, 15, 255);
        usleep(397392);
        CYPRESS(0x28, 3, 4, 150, 45, 255);
        usleep(397392);
        CYPRESS(0x28, 3, 4, 150, 75, 255);
        usleep(397392);
        CYPRESS(0x28, 3, 4, 150, 110, 255);
        usleep(397392);
        CYPRESS(0x28, 3, 4, 150, 145, 255);
        usleep(397392);
    }*/

//        CYPRESS(0x28, 3, 4, 150, 75, 255);
    init_pwm();
  //  CYPRESS(0x19, 0);
  //  CYPRESS(0x28, 3, 4, 100, 50, 255);  
    int* readbyte;
    deactivate_pwm();
    printf("Wrote I2C #2 and #3 bytes\n");

    return 0;
}

