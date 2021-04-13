#include <doctest/doctest.h>
#include <yac8emul/constants.h>
#include <yac8emul/interpreter.h>

TEST_CASE("change register") {
  CHECK(static_cast<std::uint8_t>(yac8emul::cpu::reg::V0) == 0x00);
  CHECK(static_cast<std::uint8_t>(yac8emul::cpu::reg::VF) == 0x0F);
  yac8emul::cpu cpu{};
  cpu.set_register(yac8emul::cpu::reg::V0, 0xAA);
  CHECK(cpu.get_register(yac8emul::cpu::reg::V0) == 0xAA);
}

TEST_CASE("RAM initialization") {
  using namespace yac8emul;
  cpu cpu;
  CHECK(cpu.RAM[0] == constants::sprites[0]);
  CHECK(cpu.RAM[0x30] == constants::sprites[0x30]);
}

TEST_CASE("Jump Instruction") {
  using namespace yac8emul;
  cpu cpu;
  std::vector<std::uint16_t> rom_test {
    0x1206,
    0x0000,
    0x0000,
    0x6A23,
    0x6B24,
  };

  std::vector<std::uint8_t> rom;

  for (const auto &v : rom_test) {
    rom.push_back((v & 0xFF00) >> 8);
    rom.push_back(v & 0xFF);
  }

  cpu.load_rom(rom);

  CHECK(cpu.RAM[0x200] == 0x12);
  CHECK(cpu.RAM[0x206] == 0x6A);

  CHECK_THROWS_WITH(cpu.run(), "SYSCALL");

  CHECK(cpu.get_register(cpu::reg::VA) == 0x23);
  CHECK(cpu.get_register(cpu::reg::VB) == 0x24);
}