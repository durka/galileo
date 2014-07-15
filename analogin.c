/*set analog input*/
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <helloworld.h>

int analogin()
{
    echo("/sys/class/gpio/export", "37");
    echo("/sys/class/gpio/gpio37/direction", "out");
    echo("/sys/class/gpio/gpio37/value", "0");
    printf("Enabled analog input pin A0.\n");

    return 0;
}

