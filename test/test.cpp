#include <doctest/doctest.h>
#include <yac8emul/constants.h>
#include <yac8emul/interpreter.h>

std::vector<std::uint8_t> to_u8_rom(const std::vector<std::uint16_t>& rom) {
  std::vector<std::uint8_t> r{};
  for (const auto &v : rom) {
    r.push_back((v & 0xFF00) >> 8);
    r.push_back(v & 0xFF);
  }

  return r;
}

TEST_CASE("change register") {
  CHECK(static_cast<std::uint8_t>(yac8emul::cpu::reg::V0) == 0x00);
  CHECK(static_cast<std::uint8_t>(yac8emul::cpu::reg::VF) == 0x0F);
  yac8emul::cpu cpu{};
  std::uint8_t &reg = cpu.get_register(yac8emul::cpu::reg::V0);
  reg = 0xAA;
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
    0x1206, // jmp +4
    0x0000,
    0x0000,
    0x6A23, // VA = 0x23
    0x6B24, // VB = 0x24
    0x9AB0, // VA != VB?
    0x6A99, // VA = 0x99 -- this shouldn't happen since VA *is different* from VB.
    0x6CFF, // VC = 0xFF
    0x5AB0, // Skip if VA == VB?
    0x6DFF,
    0x6EFF,
  };

  std::vector<std::uint8_t> rom = to_u8_rom(rom_test);
  cpu.load_rom(rom);

  CHECK(cpu.RAM[0x200] == 0x12);
  CHECK(cpu.RAM[0x206] == 0x6A);

  CHECK_THROWS_WITH(cpu.run(), "SYSCALL");

  CHECK(cpu.get_register(cpu::reg::VA) == 0x23);
  CHECK(cpu.get_register(cpu::reg::VB) == 0x24);
  CHECK(cpu.get_register(cpu::reg::VC) == 0xFF);
  CHECK(cpu.get_register(cpu::reg::VD) == 0xFF);
}

TEST_CASE("Add instruction") {
  using namespace yac8emul;
  cpu cpu;
  std::vector<std::uint16_t> rom_test{
    0x6A01,
    0x7A02,
    0x6B01,
    0x7BFF,
  };

  auto rom = to_u8_rom(rom_test);
  cpu.load_rom(rom);

  CHECK_THROWS_WITH(cpu.run(), "SYSCALL");

  CHECK(cpu.get_register(cpu::reg::VA) == 0x03);
  // Test overflow
  CHECK(cpu.get_register(cpu::reg::VB) == 0x00);
}

TEST_CASE("RegOP instruction") {
  using namespace yac8emul;
  cpu cpu;
  std::vector<std::uint16_t> rom_test{
      0x6A01,
      0x6B02,
      0x8AB0
  };
  auto rom = to_u8_rom(rom_test);
  cpu.load_rom(rom);
  CHECK_THROWS_WITH(cpu.run(), "SYSCALL");
  CHECK(cpu.get_register(cpu::reg::VA) == 0x02);
  CHECK(cpu.get_register(cpu::reg::VB) == 0x02);

  cpu = {};
  rom_test = {
      0x6A02,
      0x6BFF,
      0x8AB4
  };
  rom = to_u8_rom(rom_test);
  cpu.load_rom(rom);
  CHECK_THROWS_WITH(cpu.run(), "SYSCALL");
  CHECK(cpu.get_register(cpu::reg::VA) == 0x01);
  CHECK(cpu.get_register(cpu::reg::VB) == 0xFF);
  CHECK(cpu.get_register(cpu::reg::VF) == 0x1);
}