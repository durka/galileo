/*set a pwm output*/
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "i2c.h"
#include "dsp.h"
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
//based on opeartion of cypress pwm
double getfreq(int clkno, int T, int div)
{
    double clkf;
    double scale;
    double T_lenght;
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

int pwm()
{
    int cypress = i2c_openadapter(0);
    printf("Opened cypress: %d\n", cypress);
    
    //PWM select, clock source, period, pulse width, f dividier
    //format:reg1, val-of-reg1, val-of-reg2, val-of-reg3 ...
/*40 kHz pwm
    WCYPRESS(0x28, 2, 1, 3, 2);
    WCYPRESS(0x28, 3, 5, 205, 100);
*/

    int clkid = 4;
    int T = 2;
    int div = 255;
    WCYPRESS(0x28, 3, clkid, T, 1, div);

    printf("freq: %f\n", getfreq(clkid, T, div));
    printf("wrote i2c #2 and #3 bytes\n");
    //int r = read_reg(cypress, 0x20, sizeof readbyte, &readbyte);

//read interrupt register
    union i2c_smbus_data read_data;//data is copied into here;
    struct i2c_smbus_ioctl_data read_args;

    read_args.read_write = I2C_SMBUS_READ;
    read_args.command = 0x29; //interrupt register address 
    read_args.size = I2C_SMBUS_BYTE;
    read_args.data = &read_data;
    int r = ioctl(cypress, I2C_SMBUS, &read_args);  

    printf("ret: %d. read: %d\n ", r, read_data.byte);

    return 0;
}

