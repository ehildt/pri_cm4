#include <fstream>
#include <sstream>

using std::getline;
using std::ifstream;
using std::runtime_error;
using std::string;
using std::stringstream;

#define vfs "/sys/class/thermal/thermal_zone0/temp"

/**
 * reads cpu temp from virtual filesystem
 * /sys/class/thermal/thermal_zone0/temp
 * @return cpu temp in milli celsius
 */
int getCpuTemp() {
  auto ifs = ifstream{vfs};

  if (!ifs.good())
    throw runtime_error(string("Error: unable to open file ").append(vfs));

  int num;
  string line;
  stringstream ss;

  getline(ifs, line);
  ifs.close();

  ss << line;
  ss >> num;
  return num;
}
