#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "pwm.h"
#include "i2c.h"
#include "analogin.h"

int main(int argc, char const* argv[])
{
    printf("Hello, world!\n");
    printf("Bye, friends!\n");
    int i = analogin();
   // int i = pwm();
    return 0;
}

