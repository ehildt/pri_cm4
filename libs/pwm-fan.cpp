
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
#define SLEEP_TIME_NANOSECONDS 175000000
#define ZERO 0
#define TEMPERATURE_TARGET 45000

using std::cout;
using std::endl;
using std::hex;
using std::runtime_error;
using std::string;
using std::stringstream;

struct timespec SLEEP_TIME = {ZERO, SLEEP_TIME_NANOSECONDS};
char PWM_FAN_BUFFER[] = {I2C_PWM_FAN_CHIP_REGISTER, I2C_PWM_FAN_INIT_VALUE};

void control_pwm_fan() {
  reset_pwm_fan();
  do {
    int new_celsius = get_cpu_temp();
    bool temp_limit_reached = new_celsius > TEMPERATURE_TARGET;
    temp_limit_reached ? accelerate_pwm_fan(5) : decelerate_pwm_fan(5);
    nanosleep(&SLEEP_TIME, &SLEEP_TIME);
  } while (true);
}

int init_i2c_10() {
  int fd;

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

  return fd;
}

void reset_pwm_fan() {
  int fd = init_i2c_10();
  pwrite(fd, PWM_FAN_BUFFER, 2, 0);
  close(fd);
}

void accelerate_pwm_fan(unsigned char val) {
  read_pwm_fan_speed();

  if (PWM_FAN_BUFFER[0] + val > 255)
    return;

  unsigned char speed = PWM_FAN_BUFFER[0] + val;

  cout << (int)speed << endl;

  int fd = init_i2c_10();
  PWM_FAN_BUFFER[0] = I2C_PWM_FAN_CHIP_REGISTER;
  PWM_FAN_BUFFER[1] = speed;
  pwrite(fd, PWM_FAN_BUFFER, 2, 0);
  close(fd);
}

void decelerate_pwm_fan(unsigned char val) {
  read_pwm_fan_speed();

  if (PWM_FAN_BUFFER[0] - val < 0)
    return;

  unsigned char speed = PWM_FAN_BUFFER[0] - val;

  cout << (int)speed << endl;

  int fd = init_i2c_10();
  PWM_FAN_BUFFER[0] = I2C_PWM_FAN_CHIP_REGISTER;
  PWM_FAN_BUFFER[1] = speed;
  write(fd, PWM_FAN_BUFFER, 2);
  close(fd);
}

unsigned char read_pwm_fan_speed() {
  int fd = init_i2c_10();
  pread(fd, PWM_FAN_BUFFER, 1, 0);
  close(fd);
  return PWM_FAN_BUFFER[0];
}