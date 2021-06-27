#ifndef PROCESS_H
#define PROCESS_H
#include <vector>
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // constructor
  Process(int pid);
  
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  float CalcCpuUtilization(int pid);
  
//   bool operator>(Process const& a) const;  // TODO: See src/process.cpp
  
  
  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpu_util_;
  std::vector<std::string> cpu_util_vector_;
  std::string ram_;
  long int uptime_;
  std::string uid_;
  
  
  
};

#endif