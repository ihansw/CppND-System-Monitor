#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: (DONE) Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value, mem_total, mem_free;
  
  // Read file (/proc/stat)
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // std::cout <<"key: " << key << ", value: " << value << std::endl;
        if (key == "MemTotal:") {
          mem_total = value;
        }
        if (key == "MemFree:") {
          mem_free = value;
        }
        if (!mem_total.empty() and !mem_free.empty()){
          float total_mem_used_KB = stoi(mem_total) - stoi(mem_free);
          float total_mem_used_percentage = total_mem_used_KB/stoi(mem_total);
          
          filestream.close();
          return total_mem_used_percentage;
        }
      }
    }
  }
  filestream.close();
  return -1.0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string up_time, idle_process_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time >> idle_process_time;
    //std::cout << "up_time: " << up_time << std::endl;
    return stol(up_time);
  }
  
  return 0.0;
  
}

// TODO: (DONE) Read and return CPU utilization (For each process)
vector<string> LinuxParser::CpuUtilization(int pid) { 
  // (1) Get uptime
  string line, uptime;
  std::ifstream filestream_uptime(kProcDirectory + kUptimeFilename);
  if (filestream_uptime.is_open()) {
    std::getline(filestream_uptime, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  
  // (2) Get the rest of cpu utilization times
  string value, utime, stime, cutime, cstime, starttime;
  // Read file (/proc/stat)
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      int i = 0;
      while (linestream >> value) {
        i++;
        switch(i) {
            case CPUStates::utime_  : utime = value;   break;
            case CPUStates::stime_  : stime = value;   break;
            case CPUStates::cutime_  : cutime = value;   break;
            case CPUStates::cstime_  : cstime = value;   break;
            case CPUStates::starttime_  : starttime = value;   break;
        }
      }
    }
  }
  
  vector<string> cpu_util_vector = {uptime, utime, stime, cutime, cstime, starttime};
  filestream.close();
  filestream_uptime.close();
  return cpu_util_vector; 

}

// TODO: (DONE) Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;

  // Read file (/proc/stat)
  std::ifstream filestream(kProcDirectory + "stat");

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          filestream.close();
          return stoi(value);
        }
      }
    }
  }
  filestream.close();
  return -1; 
}

// TODO: (DONE) Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;

  // Read file (/proc/stat)
  std::ifstream filestream(kProcDirectory + "stat");

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          filestream.close();
          return stoi(value);
        }
      }
    }
  }
  filestream.close();
  return -1; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, value;

  // Read file (/proc/pid)
  std::ifstream filestream(kProcDirectory + to_string(pid) + "//cmdline");
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    filestream.close();
    return value;
  }
  filestream.close();
  return string(); 
}



// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;

  // Read file (/proc/pid)
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // The udacity guideline uses "VmSize" instead of "VmRSS". But, my Udacity reviewer told me that "VmSize" is actually the virtual memory usage,
        // instead of the physical RAM usage. So, the reviewer suggested me to use "VmRSS" instead!
        if (key == "VmRSS:") {
          float ram_kb = stof(value);
          float ram_mb = ram_kb / 1024;
          std::stringstream stream;
          stream << std::fixed << std::setprecision(1) << ram_mb;

          filestream.close();
          return stream.str();
        }
      }
    }
  } 
  filestream.close();
  return string();  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;

  // Read file (/proc/pid)
  std::ifstream filestream(kProcDirectory + to_string(pid) + "//status");
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return string();  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, key, x, value;

  // Read file (/proc/pid)
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value == LinuxParser::Uid(pid)) {
          filestream.close();
          return key;
        }
      }
    }
  }
  filestream.close();
  return string();  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 

  string line, value;
  long hertz = sysconf(_SC_CLK_TCK);
  // Read file (/proc/stat)
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      int i = 0;
      while (linestream >> value) {
        i++;
        if (i == CPUStates::starttime_){
            filestream.close();
            // 22nd value (llu)
            return UpTime() - stol(value)/hertz;  
        }
      }
    }
  }
  filestream.close();
  return -1; 
}
