
#include "pwm-fan.hpp"
#include "cpu-temp.hpp"

#define I2C_PWM_FAN_CHIP_ADDRESS 0x2f
#define I2C_PWM_FAN_CHIP_REGISTER 0x30
#define I2C_PWM_FAN_MAX_VALUE 0xff
#define I2C_PWM_FAN_INIT_VALUE 0x00
#define FAN_I2C_CHL "/dev/i2c-10"
#define FAN_MAX_RPM 5000
#define FAN_MAX_DBA 19.6
#define FAN_TMP_OFFSET 0.085
#define NANOSECONDS 200000000
#define ZERO 0
#define TEMPERATURE_TARGET 50000

using std::cout;
using std::endl;
using std::hex;
using std::runtime_error;
using std::string;
using std::stringstream;

void control_pwm_fan() {
  int I2C_10_FD = -1;
  struct timespec SLEEP_TIME = {ZERO, NANOSECONDS};
  char PWM_FAN_BUFFER[] = {I2C_PWM_FAN_CHIP_REGISTER, I2C_PWM_FAN_INIT_VALUE};
  int new_celsius = get_cpu_temp();
  int old_celsius = new_celsius;

  init_i2c_10(I2C_10_FD, PWM_FAN_BUFFER);

  do {
    system("clear");
    new_celsius = get_cpu_temp();

    cout << (old_celsius + new_celsius) / 2 << endl;

    if ((old_celsius + new_celsius) / 2 > TEMPERATURE_TARGET)
      accelerate_pwm_fan(I2C_10_FD, PWM_FAN_BUFFER, 3);

    else
      decelerate_pwm_fan(I2C_10_FD, PWM_FAN_BUFFER, 15);

    old_celsius = new_celsius;
    nanosleep(&SLEEP_TIME, &SLEEP_TIME);

  } while (true);
}

void init_i2c_10(int &fd, char *buffer) {

  if ((fd = open(FAN_I2C_CHL, O_RDWR)) < 0)
    throw runtime_error(string("Error: opening device ").append(FAN_I2C_CHL));

  if (ioctl(fd, I2C_SLAVE, I2C_PWM_FAN_CHIP_ADDRESS) < 0) {
    string error_msg;
    stringstream ss;
    ss << "Error: setting " << FAN_I2C_CHL << "to chip address "
       << I2C_PWM_FAN_CHIP_ADDRESS << endl;
    ss >> error_msg;
    close(fd);
    throw runtime_error(error_msg);
  }

  buffer[0] = I2C_PWM_FAN_CHIP_REGISTER;
  buffer[1] = I2C_PWM_FAN_INIT_VALUE;
  write(fd, buffer, 2);
}

void accelerate_pwm_fan(int fd, char *buffer, char val) {
  char speed = buffer[1];

  if (speed == I2C_PWM_FAN_MAX_VALUE)
    return;

  else if ((speed + val) <= I2C_PWM_FAN_MAX_VALUE)
    speed += val;

  else if (speed > (I2C_PWM_FAN_MAX_VALUE - val) &&
           speed < I2C_PWM_FAN_MAX_VALUE)
    speed = I2C_PWM_FAN_MAX_VALUE;

  buffer[0] = I2C_PWM_FAN_CHIP_REGISTER;
  buffer[1] = speed;
  write(fd, buffer, 2);
}

void decelerate_pwm_fan(int fd, char *buffer, char val) {
  char speed = buffer[1];

  if (speed == I2C_PWM_FAN_INIT_VALUE)
    return;

  else if ((speed - val) >= I2C_PWM_FAN_INIT_VALUE)
    speed -= val;

  else if (speed > I2C_PWM_FAN_INIT_VALUE && speed <= val)
    speed = I2C_PWM_FAN_INIT_VALUE;

  buffer[0] = I2C_PWM_FAN_CHIP_REGISTER;
  buffer[1] = speed;
  write(fd, buffer, 2);
}