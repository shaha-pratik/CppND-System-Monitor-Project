#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;               // TODO: See src/process.cpp
  std::string User() const;      // TODO: See src/process.cpp
  std::string Command() const;   // TODO: See src/process.cpp
  float CpuUtilization() const;  // TODO: See src/process.cpp
  std::string Ram() const;       // TODO: See src/process.cpp
  long int UpTime() const;       // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_{};
  std::string user_{};
  std::string command_{};
  float cpuUsage_{};
  std::string ram_{};
  long uptime_{};
  enum ProcessCPUStates {
    kUtime_ = 0,
    kStime_,
    kCutime_,
    kCstime_,
    kStarttime_
  };

  void CalculateCpuUtilization();
  void CalculateUser();
  void CalculateCommand();
  void CalculateRam();
  void CalculateUpTime();
};

#endif