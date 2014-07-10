/*set a pwm output*/
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "i2c.h"

#define CYPRESS(...) { unsigned char buf[] = { __VA_ARGS__ }; CYPRESS_(buf); }
#define CYPRESS_(b) ( send( cypress, 0x20, sizeof (b), (b) ) )

void send(int adapter, unsigned char address, int len, char buf[])
{
    i2c_setslave(adapter, address);
    i2c_writebytes(adapter, buf, len);
}

void echo(char filename[], char byte)
{
    int fd = open(filename, O_WRONLY);
    char buf[] = {byte, 0};
    write(fd, buf, 1);
    close(fd);
}

int pwm()
{

    echo("/sys/class/pwm/pwmchip0/export", '3');
    echo("/sys/class/pwm/pwmchip0/pwm3/enable", '1');
    printf("Enabled PWM output on pin 3.\n");

    int cypress = i2c_openadapter(0);
    printf("Opened cypress: %d\n", cypress);

    //            PWM select, clock source, period, pulse width
    CYPRESS(0x28, 0x02,       0x01,         0x03,   0x02        );
    CYPRESS(0x28, 0x03,       0x05,         0xCD,   0x66        );
    printf("Wrote I2C #2 and #3 bytes\n");

    return 0;
}

