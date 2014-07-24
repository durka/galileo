/*set a pwm output*/
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "i2c.h"
#include "helloworld.h"
#include "i2c-dev.h"

//macros to control pwm output
#define WCYPRESS(...) { unsigned char buf[] = { __VA_ARGS__ }; WCYPRESS_(buf); }
#define WCYPRESS_(b) ( send( cypress, 0x20, sizeof (b), (b) ) )

#define RCYPRESS(...) { unsigned char buf[] = { __VA_ARGS__ }; RCYPRESS_(buf); }
#define RCYPRESS_(b) ( read( cypress, 0x20, sizeof (b), (b) ) )

void send(int adapter, unsigned char address, int len, char buf[])
{
    i2c_setslave(adapter, address);
    i2c_writebytes(adapter, buf, len);
}

int read_reg(int adapter, unsigned char address, int len, char buf[])
{
    i2c_setslave(adapter, address);
    return i2c_readbytes(adapter, buf, len);
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

    deactivate_pwm();
    init_pwm();
    WCYPRESS(0x28, 2, 1, 3, 2);
    WCYPRESS(0x28, 3, 5, 205, 100);
    printf("wrote i2c #2 and #3 bytes\n");
//    deactivate_pwm();
    //int r = read_reg(cypress, 0x20, sizeof readbyte, &readbyte);

//read interrupt register
    union i2c_smbus_data read_data;//where data is read;
    struct i2c_smbus_ioctl_data read_args;

    read_args.read_write = I2C_SMBUS_READ;
    read_args.command = 0x29; //interrupt register address 
    read_args.size = I2C_SMBUS_BYTE;
    read_args.data = &read_data;
    int r = ioctl(cypress, I2C_SMBUS, &read_args);  

    printf("ret: %d. read: %d\n ", r, read_data.byte);

    return 0;
}

