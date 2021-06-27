#include <iostream>

using namespace std;

int setGPIO(int& _pin, int& _pin_mode) {
    cout << "set pin " << _pin << " with mode " << _pin_mode;
    return _pin;
}