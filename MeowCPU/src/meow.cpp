#include <map>
#include <iostream>
#include <fstream>
#include "processor.hpp"



int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Error! Usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  Processor proc;
  proc.run(argv[1]);
}
