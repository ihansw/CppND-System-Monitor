#include "processor.h"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "linux_parser.h"

using namespace std;
using namespace LinuxParser;

// TODO: (DONE) Return the aggregate CPU utilization
float Processor::Utilization() { 
  	// option = 0 --> total cpu utilization
  	// option = 1 --> current cpu utilization (delta t = 500ms)
  	int option = 0;
  
  	// Initialization
  	float cpu_util_percent;
    string line, key, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
	int prev_idle, prev_non_idle, prev_total;
  
    // Read file (/proc/stat)
    ifstream filestream(kProcDirectory + "stat");

    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        istringstream linestream(line);
        while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
          if (key == "cpu") {
            // option 0
            if (option == 0){
              int idle2 = stoi(idle) + stoi(iowait);
              int non_idle = stoi(user) + stoi(nice) + stoi(system) + stoi(irq) + stoi(softirq) + stoi(steal);
              int total = idle2 + non_idle;
              cpu_util_percent = float(non_idle)/total; 
  //             cout << "total: " << total << ", non_idle: " << non_idle << ", idle: " << idle2 << endl;
              return cpu_util_percent;
            }
            // option 1
            else{
              prev_idle = stoi(idle) + stoi(iowait);
              prev_non_idle = stoi(user) + stoi(nice) + stoi(system) + stoi(irq) + stoi(softirq) + stoi(steal);
              prev_total = prev_idle + prev_non_idle;
            }
          }
        }
      }
    }

  	// Pause for a short time
    chrono::milliseconds temp_pause(500);
    this_thread::sleep_for(temp_pause);
  
  	// Read file again
    ifstream filestream2(kProcDirectory + "stat");
  	if (filestream2.is_open()) {
      while (std::getline(filestream2, line)) {
        istringstream linestream(line);
        while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
          if (key == "cpu") {
              int curr_idle = stoi(idle) + stoi(iowait);
              int curr_non_idle = stoi(user) + stoi(nice) + stoi(system) + stoi(irq) + stoi(softirq) + stoi(steal);
              int curr_total = curr_idle + curr_non_idle;
              int totald = curr_total - prev_total;
              int idled = curr_idle - prev_idle;
              cpu_util_percent = (static_cast<float>(totald) - static_cast<float>(idled))/totald; 
            
              return cpu_util_percent;
          }
        }
      }
    }
            
    return 0.0; 
}
