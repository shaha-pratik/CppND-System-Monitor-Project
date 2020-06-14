#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  CalculateCpuUtilization();
  CalculateUser();
  CalculateRam();
  CalculateCommand();
  CalculateUpTime();
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuUsage_; }

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

void Process::CalculateCpuUtilization() {
  long uptime = LinuxParser::UpTime();
  vector<float> cpu_value = LinuxParser::CpuUtilization(pid_);
  float totaltime = cpu_value[ProcessCPUStates::kUtime_] +
                    cpu_value[ProcessCPUStates::kStime_] +
                    cpu_value[ProcessCPUStates::kCutime_] +
                    cpu_value[ProcessCPUStates::kCstime_];
  float seconds = uptime - cpu_value[ProcessCPUStates::kStarttime_];
  cpuUsage_ = totaltime / seconds;
}

void Process::CalculateUser() { user_ = LinuxParser::User(pid_); }

void Process::CalculateCommand() { command_ = LinuxParser::Command(pid_); }

void Process::CalculateRam() { ram_ = LinuxParser::Ram(pid_); }

void Process::CalculateUpTime() { uptime_ = LinuxParser::UpTime(Pid()); }