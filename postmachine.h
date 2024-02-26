
#include <fstream>
#include <iostream>

class PostMachine {
 private:
  int maxsteps_;
  int size_;
  bool *tape_;
  int curr_ = 0;
  int steps_ = 0;
 public:
  explicit PostMachine(std::ifstream &input);
  ~PostMachine();

  void R();
  void L();
  void V();
  void X();
  void print(std::ofstream &output);

  bool GetMark() const;
  int CheckSteps() const;
  int CheckCurr() const;
  int CheckMaxSteps_() const;
  bool applicable() const;


};
int SearchMinCommand(std::ifstream &program);
void DoProgram(std::ifstream &program, std::ofstream &output, PostMachine &PM);
void getNext(int &next,std::ifstream &program);
