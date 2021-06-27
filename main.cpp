#include <iostream>
#include "gpio/gpio.hpp"
#include "gpio/gpio.hpp"

using namespace std;

int main(void)
{
    string fruite = "apple";
    string fruite2 = "banana";
    int pin2 = 2;
    int pin_mode = 0;
    setGPIO(pin2, pin_mode);
    cout << endl << fruite << " " << fruite2 << endl;
}