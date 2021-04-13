// yac8emul.cpp : Defines the entry point for the application.
//

#include <yac8emul/interpreter.h>
#include <yac8emul/yac8emul.h>

int main() {
  yac8emul::cpu cpu{};

  cpu.parse_instruction(0x0000);
  std::cout << "Hello CMake." << std::endl;
  return 0;
}
