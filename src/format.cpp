#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  std::stringstream timestamp{"00:00:00"};
  if (seconds > 0) {
    timestamp.clear();
    int hh = seconds / (60 * 60);
    timestamp << std::setw(2) << std::setfill('0') << hh << ":";
    int mm = (seconds / 60) % 60;
    timestamp << std::setw(2) << std::setfill('0') << mm << ":";
    int ss = seconds % 60;
    timestamp << std::setw(2) << std::setfill('0') << ss;
  }

  return timestamp.str();
}