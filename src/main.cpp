// yac8emul.cpp : Defines the entry point for the application.
//

#include <yac8emul/interpreter.h>
#include <yac8emul/yac8emul.h>
#include <yac8emul/graphics.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Missing ROM" << std::endl;
    return -1;
  }

  std::cout << "Loading " << argv[1] << std::endl;

  yac8emul::cpu cpu{};
  std::vector <std::uint8_t> input_rom;

  std::ifstream input(argv[1], std::ios::binary);
  std::copy(std::istreambuf_iterator<char>(input),
      std::istreambuf_iterator<char>(),
      std::back_inserter(input_rom)
      );

  cpu.load_rom(input_rom);

  cpu.run();

  return 0;
}
