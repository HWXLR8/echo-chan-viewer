#include <helper.hpp>

#include <iomanip>
#include <sstream>

namespace Helper {
  std::string hex2str(int num, int padding) {
    std::stringstream ss;
    ss << std::setw(padding) << std::setfill('0') << std::uppercase << std::hex << num;
    return ss.str();
  }
}
