#ifndef MEOW_PREPROCESSOR_H
#define MEOW_PREPROCESSOR_H

#include "parser.hpp"
#include "meowstack.hpp"
#include <vector>
#include <map>

enum ReturnType {
  END_PROGRAM = -3,
  ERROR = -2, 
  OK = -1, 
  GOTO = 0
};

struct ReturnValue {
  ReturnType type;
  int idx; // used only for GOTO type
  ReturnValue(ReturnType tp) : type(tp), idx(0) {}
  ReturnValue(ReturnType tp, int idx) : type(tp), idx(idx) {}
};


class Command {
public:
  // return -3 - end program, -2 - error, -1 - OK(go to next command), >=0 - index of next command
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx) const = 0;

  virtual ~Command() = default;
};


class BeginCommand : public Command {
public:
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    return ReturnValue(ReturnType::OK);
  }
};

class EndCommand : public Command {
public:
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    return ReturnValue(ReturnType::END_PROGRAM);
  }
};

class PushCommand : public Command {
private: 
  int val;
public:	
  PushCommand(int value) : val(value) { }
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    stack.push(val);
    return ReturnValue(ReturnType::OK);
  }
};

class PopCommand : public Command {
public:
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    try {
      stack.pop();
      return ReturnValue(ReturnType::OK);
    } catch (const std::logic_error &e) {
      std::cout << e.what() << std::endl;
      return ReturnValue(ReturnType::ERROR);
    }
  }
};

class PushrCommand : public Command {
private:
  std::string val;
public:
  PushrCommand(const std::string &value) : val(value) { }
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    if (regs.find(val) != regs.end()) {
      stack.push(regs[val]);
      return ReturnValue(ReturnType::OK);
    } else {
      std::cout << "Error! REGISTER \"" << val << "\" is not initialized!" << std::endl;
      return ReturnValue(ReturnType::ERROR);
    }
  }
};


class PoprCommand : public Command {
private:
  std::string val;
public:
  PoprCommand(const std::string &value) : val(value) { }
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    try {
      regs[val] = stack.top();
      stack.pop();
      return ReturnValue(ReturnType::OK);
    } catch (const std::logic_error &e) {
      std::cout << e.what() << std::endl;
      return ReturnValue(ReturnType::ERROR);
    }
  }
};

class CalcCommand : public Command {
private:
  std::string op;
public:
  CalcCommand(const std::string &operation) : op(operation) { }
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    try {
      int A = stack.top();
      stack.pop();
      int B = stack.top();
      stack.pop();
      if (op == "ADD") {
        stack.push(A + B);
      } else if (op == "SUB") {
        stack.push(A - B);
      } else if (op == "MUL") {
        stack.push(A * B);
      } else if (op == "DIV") {
        stack.push(A / B);
      } else {
        return ReturnValue(ReturnType::ERROR);
      }

      return ReturnValue(ReturnType::OK);
    } catch (const std::logic_error &e) {
      std::cout << e.what() << std::endl;
      return ReturnValue(ReturnType::ERROR);
    }
  }
};


class OutCommand : public Command {
public:
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    try {
      std::cout << stack.top() << std::endl;
      stack.pop();
      return ReturnValue(ReturnType::OK);
    } catch (const std::logic_error &e) {
      std::cout << e.what() << std::endl;
      return ReturnValue(ReturnType::ERROR);
    }
  }
};


class InCommand : public Command {
public:
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    try {
      int val;
      std::cin >> val;
      stack.push(val);
      return ReturnValue(ReturnType::OK);
    } catch (const std::logic_error &e) {
      std::cout << e.what() << std::endl;
      return ReturnValue(ReturnType::ERROR);
    }
  }
};

class JMPCommand : public Command {
private:
  std::string label;
public:
  JMPCommand(const std::string &lbl) : label(lbl) { }
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    const auto &tmp = labels.find(label);
    if (tmp == labels.end()) {
      return ReturnValue(ReturnType::ERROR);
    }
    return ReturnValue(ReturnType::GOTO, tmp->second);
  }
};

class JMPIfCommand : public Command {
private:
  std::string operation;
  std::string label;
public:
  JMPIfCommand(const std::string &op, const std::string &lbl) : operation(op), label(lbl) { }
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    const auto &tmp = labels.find(label);
    if (tmp == labels.end()) {
      return ReturnValue(ReturnType::ERROR);
    }

    try {
      int A = stack.top();
      stack.pop();
      int B = stack.top();
      stack.pop();
      bool condition = false;
      if (operation == "JEQ") {
        condition = A == B;
      } else if (operation == "JNE") {
        condition = A != B;
      } else if (operation == "JA" ) {
        condition = A > B;
      } else if (operation == "JAE") {
        condition = A >= B;
      } else if (operation == "JB") {
        condition = A < B;
      } else if (operation == "JBE") {
        condition = A <= B;
      } else {
        return ReturnValue(ReturnType::ERROR);
      }

      return condition ? ReturnValue(ReturnType::GOTO, tmp->second) : ReturnValue(ReturnType::OK);
    } catch (const std::logic_error &e) {
      std::cout << e.what() << std::endl;
      return ReturnValue(ReturnType::ERROR);
    }
  }
};


class RetCommand : public Command {
public:
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    int res = callStack.top();
    callStack.pop();
    return ReturnValue(ReturnType::GOTO, res);
  }
};


class CallCommand : public Command {
private:
  std::string label;
public:
  CallCommand(const std::string &lbl) : label(lbl) { }
  virtual ReturnValue run(MS::MeowStack<int> &stack, MS::MeowStack<int> &callStack, std::map<std::string, int> &regs, const std::map<std::string, int> &labels, int commandIdx = 0) const override {
    const auto &tmp = labels.find(label);
    if (tmp == labels.end()) {
      return ReturnValue(ReturnType::ERROR);
    }
    callStack.push(commandIdx + 1);
    return ReturnValue(ReturnType::GOTO, tmp->second);
  }
};


struct Program
{
  std::vector<Command*> commands;
  std::map<std::string, int> labels; // label - index (перед какой коммандой стоит лейбл label)
  int startIdx = -1;
  bool endFlag = false;

  ~Program() {
    for (int i = 0; i < commands.size(); ++i)
    {
      delete commands[i];
    }
  }

  bool isValid() const { return commands.size() != 0; }
  void clear() {
    for (int i = 0; i < commands.size(); ++i)
    {
      delete commands[i];
    }
    commands.clear();
    labels.clear();
    startIdx = -1;
    endFlag = false;
  }
};


class Preprocessor {
public:
  Preprocessor() = default;
  ~Preprocessor() = default;

  int process(const char* file_name, Program &res) {
    Parser parser;
    parser.init(file_name);

    Token curr = parser.getToken();

    while (curr.type != TokenType::END_OF_FILE) {
      switch (curr.type) {
        case TokenType::UNDEFINED: {
          std::cout << "Error! Preprocessor:: UNDEFINED tokentype" << std::endl;
          return -1;
        }
        case TokenType::LABEL: {
          curr.value.pop_back(); // выкинули ':'
          if (res.labels.find(curr.value) != res.labels.end()) {
            std::cout << "Error! Preprocessor:: duplicate label \"" << curr.value << "\"" << std::endl;
            return -1;
          }
          res.labels[curr.value] = res.commands.size();

          break;
        }
        case TokenType::WORD: {

          if (curr.value == "BEGIN") {
            if (res.startIdx != -1) {
              std::cout << "Error! Preprocessor:: duplicate BEGIN" << std::endl;
              return -1;
            }
            res.startIdx = res.commands.size();
            res.commands.push_back(new BeginCommand());
          } else if (curr.value == "END") {
            res.endFlag = true;
            res.commands.push_back(new EndCommand());
          } else {
            Command* com = readCommand(parser, curr);
            if (com == nullptr)
            {
              return -1;
            }
            res.commands.push_back(com);
          }
          break;
        }
        case TokenType::INTEGER_VALUE: {
          std::cout << "Error! Preprocessor:: unexpected INTEGER_VALUE tokentype " << std::endl;
          return -1;
        }
      }

      curr = parser.getToken();
    }
    
    if (!res.endFlag) {
      std::cout << "Error! Preprocessor:: no END in file" << std::endl;
      return -1;
    }

    return 0;
  }

private:
  Command* readCommand(Parser &parser, Token &curr) {
    const std::string command = curr.value;

    if (command == "PUSH") {
      curr = parser.getToken();
      if (curr.type != TokenType::INTEGER_VALUE)
      {
        std::cout << "Error! Preprocessor::Command PUSH:: expected INTEGER_VALUE tokentype" << std::endl;
        return nullptr;
      }
      return new PushCommand(curr.integer);
    }

    if (command == "POP") {
      return new PopCommand();
    }

    if (command == "PUSHR") {
      curr = parser.getToken();
      if (curr.type != TokenType::WORD)
      {
        std::cout << "Error! Preprocessor::Command PUSHR:: expected WORD tokentype" << std::endl;
        return nullptr;
      }
      return new PushrCommand(curr.value);
    }

    if (command == "POPR") {
      curr = parser.getToken();
      if (curr.type != TokenType::WORD)
      {
        std::cout << "Error! Preprocessor::Command POPR:: expected WORD tokentype" << std::endl;
        return nullptr;
      }
      return new PoprCommand(curr.value);
    }

    if (   command == "ADD"
        || command == "SUB"
        || command == "MUL"
        || command == "DIV")
    {
      return new CalcCommand(command);
    }

    if (command == "OUT") {
      return new OutCommand();
    }

    if (command == "IN") {
      return new InCommand();
    }

    if (command == "JMP") {
      curr = parser.getToken();
      if (curr.type != TokenType::WORD)
      {
        std::cout << "Error! Preprocessor::Command JMP:: expected WORD tokentype" << std::endl;
        return nullptr;
      }
      return new JMPCommand(curr.value);
    }

    if (   command == "JEQ"
        || command == "JNE"
        || command == "JA"
        || command == "JAE"
        || command == "JB"
        || command == "JBE")
    {
      curr = parser.getToken();
      if (curr.type != TokenType::WORD)
      {
        std::cout << "Error! Preprocessor::Command JMP:: expected WORD tokentype" << std::endl;
        return nullptr;
      }
      return new JMPIfCommand(command, curr.value);
    }


    if (command == "CALL") {
      curr = parser.getToken();
      if (curr.type != TokenType::WORD)
      {
        std::cout << "Error! Preprocessor::Command JMP:: expected WORD tokentype" << std::endl;
        return nullptr;
      }
      return new CallCommand(curr.value);
    }

    if (command == "RET") {
      return new RetCommand();
    }

    return nullptr;
  }
};


#endif // MEOW_PREPROCESSOR_H
