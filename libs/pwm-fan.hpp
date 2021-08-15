#ifndef _I2C_10_PWM_FAN_
#define _I2C_10_PWM_FAN_

#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>

void control_pwm_fan();
void init_i2c_10(int &, char *);
void accelerate_pwm_fan(int, char *, char);
void decelerate_pwm_fan(int, char *, char);

#endif