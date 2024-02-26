#include "postmachine.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Missing arguments: program path, input path, output path";
    return 1;
  }
  std::ifstream program(argv[1]);
  std::ifstream input(argv[2]);
  std::ofstream output(argv[3]);

  PostMachine PM(input);
  input.close();

  DoProgram(program,output,PM);

  program.close();
  output.close();

  return 0;
}
