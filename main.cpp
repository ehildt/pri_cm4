#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::string;
using std::to_string;

int main(void) {
  struct timespec SLEEP_TIME = {0, 500000000};

  do {
    int FD_PWM_FAN;
    char I2C_PWM_FAN_CHIP_ADDRESS = 0x2f;
    char I2C_PWM_FAN_CHIP_REGISTER = 0x30;
    char I2C_PWM_FAN_INIT_VALUE = 0x20;
    string FILE_NAME = "/dev/i2c-10";
    char BUFFER[] = {I2C_PWM_FAN_CHIP_REGISTER, I2C_PWM_FAN_INIT_VALUE};

    if ((FD_PWM_FAN = open(FILE_NAME.c_str(), O_RDWR)) < 0)
      exit(1);

    if (ioctl(FD_PWM_FAN, I2C_SLAVE, I2C_PWM_FAN_CHIP_ADDRESS) < 0)
      exit(1);

    if (pwrite(FD_PWM_FAN, BUFFER, 2, 0) < 0)
      exit(1);

    pread(FD_PWM_FAN, BUFFER, 1, 0);
    close(FD_PWM_FAN);
    const float sum = (int)BUFFER[0] / 255.0 * 100;
    string tmp = " \"fan: %.2f\n\" " + to_string(sum);
    system(string("printf").append(tmp).c_str());

    nanosleep(&SLEEP_TIME, &SLEEP_TIME);
  } while (true);
}