#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "libs/cpu-temp.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::runtime_error;
using std::string;
using std::to_string;

#define I2C_PWM_FAN_CHIP_ADDRESS 0x2f
#define I2C_PWM_FAN_CHIP_REGISTER 0x30
#define I2C_PWM_FAN_MAX_VALUE 0xff
#define I2C_PWM_FAN_INIT_VALUE 0x00
#define FAN_I2C_CHL "/dev/i2c-10"
#define FAN_MAX_RPM 5000
#define FAN_MAX_DBA 19.6
#define FAN_TMP_OFFSET 0.085

int main(void) {
  struct timespec SLEEP_TIME = {0, 175000000};

  int FD_PWM_FAN;
  char BUFFER[] = {I2C_PWM_FAN_CHIP_REGISTER, I2C_PWM_FAN_INIT_VALUE};

  int milli_celsius = getCpuTemp();
  float celsius = milli_celsius / 1000.0;
  float fallingNum = 0;

  if ((FD_PWM_FAN = open(FAN_I2C_CHL, O_RDWR)) < 0)
    exit(1);

  if (ioctl(FD_PWM_FAN, I2C_SLAVE, I2C_PWM_FAN_CHIP_ADDRESS) < 0)
    exit(1);

  if (pwrite(FD_PWM_FAN, BUFFER, 2, 0) < 0)
    exit(1);

  do {

    if ((FD_PWM_FAN = open(FAN_I2C_CHL, O_RDWR)) < 0)
      exit(1);

    if (ioctl(FD_PWM_FAN, I2C_SLAVE, I2C_PWM_FAN_CHIP_ADDRESS) < 0)
      exit(1);

    pread(FD_PWM_FAN, BUFFER, 1, 0);

    const float sum = BUFFER[0] / 255.0 * 100;
    const float fan_dba = FAN_MAX_DBA * (sum / 100);
    const short fan_rpm = FAN_MAX_RPM * (sum / 100);
    string fanSpeed = " \"Fan: %.2f%%, %drpm, %.2fdb(a), celsius %.2f\n\" " +
                      to_string(sum) + " " + to_string(fan_rpm) + " " +
                      to_string(fan_dba) + " " + to_string(celsius);
    system(string("printf").append(fanSpeed).c_str());

    milli_celsius = getCpuTemp();
    celsius = milli_celsius / 1000.0;

    fallingNum = celsius > fallingNum ? celsius : (fallingNum - FAN_TMP_OFFSET);

    if (milli_celsius > 45000 && BUFFER[1] <= 240 && celsius >= fallingNum) {
      BUFFER[0] = I2C_PWM_FAN_CHIP_REGISTER;
      BUFFER[1] += 15;
      pwrite(FD_PWM_FAN, BUFFER, 2, 0);
      close(FD_PWM_FAN);
    }

    if (BUFFER[1] >= 3) {
      BUFFER[0] = I2C_PWM_FAN_CHIP_REGISTER;
      BUFFER[1] -= 3;
      pwrite(FD_PWM_FAN, BUFFER, 2, 0);
      close(FD_PWM_FAN);
    }

    nanosleep(&SLEEP_TIME, &SLEEP_TIME);

  } while (true);
}
