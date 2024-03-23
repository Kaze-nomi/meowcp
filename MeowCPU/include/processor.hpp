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
      ReturnValue res = prog.commands[idx]->run(stack, callStack, regs, prog.labels, idx);
      switch (res.type) {
      case ReturnType::END_PROGRAM:
        return 0;
        break;
      case ReturnType::ERROR:
        return -1;
        break;
      case ReturnType::OK:
        idx++;
        break;
      case ReturnType::GOTO:
        idx = res.idx;
        break;
      }
    }
  }
};


#endif // MEOW_PROCESSOR_H