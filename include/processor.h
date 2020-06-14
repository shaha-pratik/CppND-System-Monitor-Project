#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prevTotalCpuTime{0.0f};
  float prevIdleCpuTime{0.0f};
};

#endif