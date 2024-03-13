#include "meowstack.hpp"
#include <map>
#include <iostream>
#include <fstream>



int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Error! Usage: " << argv[0] << " filename" << std::endl;
        return 1;
    }
    std::fstream in;
    in.open(argv[1], std::ios::in);

    if (!in.is_open()) {
        std::cout << "Error! File \"" << argv[1] << "\" not found" << std::endl;
        return 2;
    }
    
    std::string command;

    in >> command;

    while (!in.eof() && command != "BEGIN") {
        in >> command;
    }

    if (command != "BEGIN") {
        std::cout << "Error! BEGIN is not found in file \"" << argv[1] << "\"" << std::endl;
        return 3;
    }
    
    std::map<std::string, int> registers;

    MS::MeowStack<int> stack;

    in >> command;
    
    while (!in.eof() && command != "END") {
        if (command == "PUSH") {
            int value;
            in >> value;
            stack.push(value);
        }

        if (command == "POP") {
            stack.pop();
        }

        if (command == "PUSHR") {
            std::string reg;
            in >> reg;
            if (registers.find(reg) == registers.end()) {
                std::cout << "Error! REGISTER is not found in file \"" << argv[1] << "\"" << std::endl;
                return 3;
            }

            stack.push(registers[reg]);
        }

        if (command == "POPR") {
            std::string reg;
            in >> reg;

            registers[reg] = stack.top();

            stack.pop();
        }

        if (command == "ADD") {
            int A = stack.top();
            stack.pop();
            int B = stack.top();
            stack.push(A);
            stack.push(A+B);
        }

        if (command == "SUB") {
            int A = stack.top();
            stack.pop();
            int B = stack.top();
            stack.push(A);
            stack.push(A-B);
        }

        if (command == "MUL") {
            int A = stack.top();
            stack.pop();
            int B = stack.top();
            stack.push(A);
            stack.push(A*B);
        }

        if (command == "DIV") {
            int A = stack.top();
            stack.pop();
            int B = stack.top();
            stack.push(A);
            stack.push(A/B);
        }

        if (command == "OUT") {
            std::cout << stack.top() << std::endl;
            stack.pop();
        }

        if (command == "IN") {
            int val;
            std::cin >> val;
            stack.push(val);
        }
        
        in >> command;
    }
    
    if (command != "END") {
        std::cout << "Error! END is not found in file \"" << argv[1] << "\"" << std::endl;
        return 3;
    }
}
