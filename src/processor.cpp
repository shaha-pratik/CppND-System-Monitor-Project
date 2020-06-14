#include <algorithm>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto cpu_values_default = LinuxParser::CpuUtilization();
  vector<long> cpu_values(cpu_values_default.size());
  std::transform(cpu_values_default.begin(), cpu_values_default.end(),
                 cpu_values.begin(),
                 [](const string& s) -> long { return std::stol(s); });

  float totalCpuTime =
      cpu_values[LinuxParser::kUser_] + cpu_values[LinuxParser::kNice_] +
      cpu_values[LinuxParser::kSystem_] + cpu_values[LinuxParser::kIdle_] +
      cpu_values[LinuxParser::kIOwait_] + cpu_values[LinuxParser::kIRQ_] +
      cpu_values[LinuxParser::kSoftIRQ_] + cpu_values[LinuxParser::kSteal_];

  float idleCpuTime =
      cpu_values[LinuxParser::kIdle_] + cpu_values[LinuxParser::kIOwait_];

  float diffIdle = idleCpuTime - prevIdleCpuTime;
  float diffTotal = totalCpuTime - prevTotalCpuTime;
  float diffUsage = (diffTotal - diffIdle) / diffTotal;

  prevIdleCpuTime = idleCpuTime;
  prevTotalCpuTime = totalCpuTime;
  return diffUsage;
}
