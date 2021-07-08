#ifndef _I2C_10_PWM_FAN_
#define _I2C_10_PWM_FAN_

#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>

void reset_pwm_fan();
void control_pwm_fan();
unsigned char read_pwm_fan_speed();
void accelerate_pwm_fan(unsigned char);
void decelerate_pwm_fan(unsigned char);

#endif