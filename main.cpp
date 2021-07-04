

#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

using std::cerr;
using std::cout;
using std::string;
using std::to_string;

int main(void)
{
    do
    {
        char16_t TEMP_BUFFER;
        int TEMP_DESCRIPTOR;
        string TEMP_NAME = "/sys/devices/virtual/thermal/thermal_zone0/temp";
        if ((TEMP_DESCRIPTOR = open(TEMP_NAME.c_str(), O_RDONLY)) < 0)
            exit(1);

        read(TEMP_DESCRIPTOR, &TEMP_BUFFER, sizeof(char16_t));
        close(TEMP_DESCRIPTOR);
        int myval = TEMP_BUFFER;
        string temperature = string("echo temp ").append(to_string(myval));
        system(temperature.c_str());

        int FILE_DESCRIPTOR;
        char I2C_CHIP_ADDRESS = 0x2f;
        char I2C_CHIP_REGISTER = 0x30;
        char I2C_INIT_VALUE = 0x00;
        string FILE_NAME = "/dev/i2c-10";
        char BUFFER[] = {I2C_CHIP_REGISTER, I2C_INIT_VALUE};

        if ((FILE_DESCRIPTOR = open(FILE_NAME.c_str(), O_RDWR)) < 0)
            exit(1);

        if (ioctl(FILE_DESCRIPTOR, I2C_SLAVE, I2C_CHIP_ADDRESS) < 0)
            exit(1);

        if (pwrite(FILE_DESCRIPTOR, BUFFER, 2, 0) < 0)
            exit(1);

        pread(FILE_DESCRIPTOR, BUFFER, 1, 0);
        close(FILE_DESCRIPTOR);
        const float sum = (int)BUFFER[0] / 255.0 * 100;
        string tmp = " \"fan: %.2f\n\" " + to_string(sum);
        system(string("printf").append(tmp).c_str());

        sleep(1);
    } while (true);
}