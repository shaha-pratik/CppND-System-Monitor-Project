#include <dirent.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T LinuxParserHelper(const string& filepath, const string& key) {
  string line{}, actual_key{};
  T value{};
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> actual_key >> value) {
        if (key.compare(actual_key) == 0) {
          return value;
        }
      }
    }
  }
  return value;
}

template <typename T>
T LinuxParserHelper(const string& filepath) {
  string line{};
  T value{};
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> value;
    }
  }
  return value;
}
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
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os{}, version{}, kernel{};
  string line{};
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string filepath = kProcDirectory + kMeminfoFilename;
  double total_mem_info = LinuxParserHelper<double>(filepath, "MemTotal:");
  double free_mem_info = LinuxParserHelper<double>(filepath, "MemFree:");
  return (total_mem_info - free_mem_info) / (total_mem_info);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string filename = kProcDirectory + kUptimeFilename;
  return LinuxParserHelper<long>(filename);
}

// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }
// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }
// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }
// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      vector<string> cpu_values{std::istream_iterator<string>{linestream},
                                std::istream_iterator<string>{}};
      cpu_values.erase(cpu_values.begin());
      return cpu_values;
    }
  }
  return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string filename = kProcDirectory + kStatFilename;
  return LinuxParserHelper<int>(filename, "processes");
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string filename = kProcDirectory + kStatFilename;
  return LinuxParserHelper<int>(filename, "procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string filepath = kProcDirectory + to_string(pid) + kCmdlineFilename;
  return LinuxParserHelper<string>(filepath);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string filepath = kProcDirectory + to_string(pid) + kStatusFilename;
  auto value = LinuxParserHelper<long>(filepath, "VmSize:") / 1024.0f;
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << value;
  return ss.str();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string filepath = kProcDirectory + to_string(pid) + kStatusFilename;
  return LinuxParserHelper<string>(filepath, "Uid:");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string filepath = kPasswordPath;
  string line{}, key{}, x{}, value{};
  std::ifstream filestream(filepath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (uid.compare(value) == 0) {
          return key;
        }
      }
    }
  }
  return key;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string filename = kProcDirectory + to_string(pid) + kStatFilename;
  std::ifstream filestream(filename);
  string line;
  long pid_time{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    vector<string> data{std::istream_iterator<string>{linestream},
                        std::istream_iterator<string>{}};
    pid_time = std::stol(data[21]);
  }
  return LinuxParser::UpTime() - pid_time / sysconf(_SC_CLK_TCK);
}

std::vector<float> LinuxParser::CpuUtilization(const int pid) {
  std::vector<float> process_info = {};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  std::string line{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    const std::vector<std::string> data{
        std::istream_iterator<string>{linestream},
        std::istream_iterator<string>{}};

    process_info.push_back(std::stol(data[13]) / sysconf(_SC_CLK_TCK));
    process_info.push_back(std::stol(data[14]) / sysconf(_SC_CLK_TCK));
    process_info.push_back(std::stol(data[15]) / sysconf(_SC_CLK_TCK));
    process_info.push_back(std::stol(data[16]) / sysconf(_SC_CLK_TCK));
    process_info.push_back(std::stol(data[21]) / sysconf(_SC_CLK_TCK));
    return process_info;
  }
  return {};
}
