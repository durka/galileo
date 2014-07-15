/*set a pwm output*/
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "i2c.h"
#include "UtilTime.h"
#include "helloworld.h"

//macros to control pwm output
#define CYPRESS(...) { unsigned char buf[] = { __VA_ARGS__ }; CYPRESS_(buf); }
#define CYPRESS_(b) ( send( cypress, 0x20, sizeof (b), (b) ) )

void send(int adapter, unsigned char address, int len, char buf[])
{
    i2c_setslave(adapter, address);
    i2c_writebytes(adapter, buf, len);
}

int pwm()
{
    int cypress = i2c_openadapter(0);
    printf("Opened cypress: %d\n", cypress);
    //PWM select, clock source, period, pulse width, f dividier
    //format:reg1, val-of-reg1, val-of-reg2, val-of-reg3 ...
    CYPRESS(0x28, 5, 1, 3, 2);
    CYPRESS(0x28, 6, 5, 205, 50);
   // delay(1000);
//    CYPRESS(0x28, 6, 5, 205, 150)
/*
    double clk = 32000;
    double clksrc = 0;
    double div = 1;
    clk = clk/div;
    double period = 10;
    double pw = 5;
    double scale = 0.813;
    double f = scale*clk/period; 
    double T = 1/f;
    int i;
    CYPRESS(0x28, 5, clksrc, period, pw);
    while (true) {
    for(i = 0; i < 10; i++){
        CYPRESS(0x28, 4, clksrc, period, i, div);
        delay(T/1000);
    }
    }*/
  //  printf("f: %f\n", f);
    printf("Wrote I2C #2 and #3 bytes\n");

    return 0;
}

