
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

// example for /dev/i2c-10
// command: i2cset -y 10 0x2f 0x30 0x00
// -y: disable interactive mode (don't wait for the user)
// 10: the number or name of the I2C bus to be scanned
// 0x2f: the address of the chip on that bus
// 0x30: the address on that chip to write to
// 0x00: the value to write to that location on the chip

using std::hex;
using std::to_string;
using std::string;
using std::ofstream;
using std::stringstream;

int main(void)
{

    ofstream file("/dev/i2c-10");
    if (file.is_open())
    {
        int byte = 0;
        while (true)
        {
            string command = "i2cset -y 10 0x2f 0x30 0x";
            if(byte < 255) byte++;
            float sum = (byte / 255.0) * 100;
            string sum_str = to_string(sum);
            stringstream sstream;
            sstream << hex << byte;
            system(command.append(sstream.str()).c_str());
            system(string("echo ").append(command).append(" ").append(sum_str).c_str());
            sleep(1);
        }
    }
    return -1;
}