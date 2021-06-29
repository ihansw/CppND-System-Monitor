#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// constructor
Process::Process(int pid){
  pid_ = pid;
  user_ = LinuxParser::User(pid);
  command_ = LinuxParser::Command(pid);
  cpu_util_ = CalcCpuUtilization(pid);
  ram_ = LinuxParser::Ram(pid);
  uptime_ = LinuxParser::UpTime(pid);
  uid_ = LinuxParser::Uid(pid);
  
//   if(pid == 1544){
//   	std::cout << "pid_: " << pid_ << ", user_: " << user_ << ", command_: " << command_ << ", cpu_util_: " << cpu_util_ << ", ram_: " << ram_ << ", uptime_: " << uptime_ << std::endl;
//   }
}
// TODO: (DONE) Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_util_; }

float Process::CalcCpuUtilization(int pid) {
  cpu_util_vector_ = LinuxParser::CpuUtilization(pid);
  
  long hertz = sysconf(_SC_CLK_TCK);
  float cpu_usage;
  try {
    long uptime = stol(cpu_util_vector_[0]);
    long utime = stol(cpu_util_vector_[1]);
    long stime = stol(cpu_util_vector_[2]);
    long cutime = stol(cpu_util_vector_[3]);
    long cstime = stol(cpu_util_vector_[4]);
    long starttime = stol(cpu_util_vector_[5]);
    
    long total_cpu_time_jiffies = utime + stime + cutime + cstime;
    float total_cpu_time_seconds = total_cpu_time_jiffies / hertz;
    float total_run_time_seconds = uptime - (starttime / hertz);
    cpu_usage = (total_cpu_time_seconds / total_run_time_seconds);
  } catch(...){
    cpu_usage = 0;
  }

  return cpu_usage;
}
// TODO: (DONE) Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function

bool Process::operator<(Process const& a) const {
  return cpu_util_ < a.cpu_util_;
}



// bool Process::operator>(Process const& a) const {
//   return cpu_util_ > a.cpu_util_;
// }