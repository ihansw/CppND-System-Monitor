#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;



// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  
  // clear previous processes
  processes_.clear();
  
  // extract current processes
  vector<int> pid_list = LinuxParser::Pids();
  int pid_list_size = pid_list.size();
  
  for(int i = 0; i < pid_list_size ; i++){
  	Process new_process(pid_list[i]);
    processes_.push_back(new_process); 
  }
  
  // Sort the list.
  struct greater{
	bool operator() (Process& a, Process& b) { return (a.CpuUtilization() > b.CpuUtilization()); }
  };
  
  std::sort(processes_.begin(), processes_.end(), greater());
  
  return processes_; 
}

// TODO: (DONE) Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// TODO: (DONE) Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// TODO: (DONE) Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

// TODO: (DONE) Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// TODO: (DONE) Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// TODO: (DONE) Return the number of seconds since the system started running
long int System::UpTime() { 
  	return LinuxParser::UpTime(); 
}