#ifndef MEOW_PROCESSOR_H
#define MEOW_PROCESSOR_H

#include "preprocessor.hpp"

#include <vector>
#include <map>


class Processor {
  MS::MeowStack<int> stack;
  MS::MeowStack<int> callStack;
  std::map<std::string, int> regs;

public:
  int run(const char *file_name) {
    Preprocessor preproc;
    Program prog;
    if (preproc.process(file_name, prog) == -1) {
      return -1;
    }
    int idx = prog.startIdx;
    while (true) {
      int res = prog.commands[idx]->run(stack, callStack, regs, prog.labels, idx);
      if (res == -3) {
        return 0;
      }
      if (res == -2) {
        return -1;
      }
      if (res == -1) {
        idx++;
      }
      if (res >= 0) {
        idx = res;
      }
    }
  }
};


#endif // MEOW_PROCESSOR_H