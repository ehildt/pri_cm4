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

#define I2C_PWM_FAN_CHIP_ADDRESS 0x2f
#define I2C_PWM_FAN_CHIP_REGISTER 0x30
#define I2C_PWM_FAN_INIT_VALUE 0x66
#define FAN_I2C_CHL "/dev/i2c-10"
#define FAN_MAX_RPM 5000
#define FAN_MAX_DBA 19.6

int main(void) {
  struct timespec SLEEP_TIME = {0, 500000000};

  do {
    int FD_PWM_FAN;
    char BUFFER[] = {I2C_PWM_FAN_CHIP_REGISTER, I2C_PWM_FAN_INIT_VALUE};

    if ((FD_PWM_FAN = open(FAN_I2C_CHL, O_RDWR)) < 0)
      exit(1);

    if (ioctl(FD_PWM_FAN, I2C_SLAVE, I2C_PWM_FAN_CHIP_ADDRESS) < 0)
      exit(1);

    if (pwrite(FD_PWM_FAN, BUFFER, 2, 0) < 0)
      exit(1);

    pread(FD_PWM_FAN, BUFFER, 1, 0);
    close(FD_PWM_FAN);
    const float sum = (int)BUFFER[0] / 255.0 * 100;
    const float fan_dba = FAN_MAX_DBA * (sum / 100);
    const short fan_rpm = FAN_MAX_RPM * (sum / 100);
    string fanSpeed = " \"Fan: %.2f%%, %drpm, %.2fdb(a) \n\" " +
                      to_string(sum) + " " + to_string(fan_rpm) + " " +
                      to_string(fan_dba);
    system(string("printf").append(fanSpeed).c_str());

    nanosleep(&SLEEP_TIME, &SLEEP_TIME);
  } while (true);
}