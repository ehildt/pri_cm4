
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <iostream>

using std::to_string;

int main(void)
{
    int file;
    const char *filename = "/dev/i2c-10";
    if ((file = open(filename, O_RDWR)) < 0)
    {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    int addr = 0x2f; // The I2C address of the ADC
    int reg = 0x30;
    if (ioctl(file, I2C_SLAVE, addr) < 0)
    {
        printf("Failed to acquire bus access %d on %d and/or talk to slave.\n", addr, reg);
        exit(1);
    }

    char buf[2] = {(char)reg, (char)125};

    if (write(file, buf, 2) < 0)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to read from the i2c bus.\n");
        printf("output %s", buf);
        printf("\n\n");
        exit(1);
    }

    buf[0] = 2;

    if (read(file, buf, 1) < 0)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to read from the i2c bus.\n");
        printf(buf);
        printf("\n\n");
        exit(1);
    }

    while (true)
    {
        const float sum = buf[0] / 255.0 * 100;
        std::string tmp = "echo " + to_string(sum);
        system(tmp.c_str());
        sleep(1);
    }
}