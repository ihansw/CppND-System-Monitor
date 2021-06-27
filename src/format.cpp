#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hr = seconds/3600;
  int min = (seconds/60) % 60;
  int sec = seconds % 60;
  
  stringstream ss;
  ss << std::setfill('0') << std::setw(2) << hr << ':' << std::setw(2) << min  << ':' << std::setw(2) << sec;
  return ss.str();
}