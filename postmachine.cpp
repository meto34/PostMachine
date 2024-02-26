#include "postmachine.h"

PostMachine::~PostMachine() {
  delete[] tape_;
  tape_ = nullptr;
  size_ = 0;
  curr_ = 0;
  steps_ = 0;
}

bool PostMachine::GetMark() const { return tape_[curr_]; }

int PostMachine::CheckSteps() const { return steps_; }

int PostMachine::CheckCurr() const { return curr_; }

int PostMachine::CheckMaxSteps_() const { return maxsteps_; }

void PostMachine::R() {
  if (curr_ != size_ - 1) {
    ++curr_;
  } else {
    curr_ = 0;
  }
  ++steps_;
}

void PostMachine::L() {
  if (curr_ != 0) {
    --curr_;
  } else {
    curr_ = size_ - 1;
  }
  ++steps_;
}

void PostMachine::V() {
  tape_[curr_] = true;
  ++steps_;
}

void PostMachine::X() {
  tape_[curr_] = false;
  ++steps_;
}

bool PostMachine::applicable() const { return steps_ <= maxsteps_; }

void PostMachine::print(std::ofstream &output) {
  int zeros = 0;
  int curr = curr_;
  for (int i = 0; i < size_; ++i) {
    if (tape_[curr]) {
      for (int j = 0; j < zeros; ++j) {
        output << '0';
      }
      zeros = 0;
      output << '1';
    } else {
      ++zeros;
    }
    if (curr + 1 == size_) {
      curr = 0;
    } else {
      ++curr;
    }
  }
}

PostMachine::PostMachine(std::ifstream &input) {
  bool CycleIsCompleted = false;
  input >> size_;
  input >> maxsteps_;
  input.get();
  tape_ = new bool[size_];
  int i = 0;
  while (input.peek() != EOF) {
    if ((input.peek() == '1') or (input.peek() == '0')) {
      tape_[i] = ((input.get() - '0') != 0);
      if (i != size_ - 1) {
        ++i;
      } else {
        i = 0;
        CycleIsCompleted = true;
      }
    }
  }
  if (!CycleIsCompleted) {
    for (; i < size_ - 1; ++i) {
      tape_[i] = false;
    }
  }
}

int SearchMinCommand(std::ifstream &program) {
  int curr;
  int next = 2147483647;
  while (program.peek() != EOF) {
    if (program.peek() == '%') {
      while ((program.peek() != EOF) && (program.peek() != '\n')) {
        program.get();
      }
      if (program.peek() == '\n') {
        program.get();
      }
      if (program.peek() == EOF) {
        return next;
      }
    } else {
      program >> curr;
      if (curr < next) {
        next = curr;
      }
      while ((program.peek() != EOF) && (program.peek() != '\n')) {
        program.get();
      }
      if (program.peek() == '\n') {
        program.get();
      }
      if (program.peek() == EOF) {
        return next;
      }
    }
  }
  return next;
}

void DoProgram(std::ifstream &program, std::ofstream &output, PostMachine &PM) {
  int next = SearchMinCommand(program);
  program.clear();
  program.seekg(0);
  int curr;
  while (program.peek() != EOF) {
    if (program.peek() == '%') {
      while ((program.peek() != EOF) && (program.peek() != '\n')) {
        program.get();
      }
      if (program.peek() == '\n') {
        program.get();
      }
      if (program.peek() == EOF) {
        output << "Error";
        return;
      }
    } else {
      program >> curr;
      program.get();  // get '.'
      program.get();  // get // ' '
      if (curr == next) {
        switch (program.get()) {
          case 'R':
            PM.R();
            getNext(next, program);
            if (PM.applicable()) {
              break;
            }
            output << "Not applicable";  // else
            return;

          case 'V':
            PM.V();
            getNext(next, program);
            if (PM.applicable()) {
              break;
            }
            output << "Not applicable";
            return;

          case 'L':
            PM.L();
            getNext(next, program);
            if (PM.applicable()) {
              break;
            }
            output << "Not applicable";
            return;

          case 'X':
            PM.X();
            getNext(next, program);
            if (PM.applicable()) {
              break;
            }
            output << "Not applicable";
            return;

          case '!':
            PM.print(output);
            return;
          case '?':
            if (PM.GetMark()) {
              program.get();  // get ' '
              program.get();  // get 'j1'
              getNext(next, program);
              program.clear();
              program.seekg(0);
              if (PM.applicable()) {
                break;
              }
              output << "Not applicable";
              return;

            } else {
              getNext(next, program);
              if (PM.applicable()) {
                break;
              }
              output << "Not applicable";
              return;
            }
          default:
            output << "Error";
            return;
        }
      } else {
        while ((program.peek() != EOF) && (program.peek() != '\n')) {
          program.get();
        }
        if (program.peek() == '\n') {
          program.get();
        }
        if (program.peek() == EOF) {
          output << "Error";
          return;
        }
      }
    }
  }
}

void getNext(int &next, std::ifstream &program) {
  program.get();  // get ' '
  program >> next;
  program.clear();
  program.seekg(0);
}