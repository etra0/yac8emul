#include <doctest/doctest.h>
#include <yac8emul/interpreter.h>
#include <yac8emul/constants.h>

TEST_CASE("change register") {
    CHECK(static_cast<std::uint8_t>(yac8emul::cpu::reg::V0) == 0x00);
    CHECK(static_cast<std::uint8_t>(yac8emul::cpu::reg::VF) == 0x0F);
    yac8emul::cpu cpu{};
    cpu.change_register(yac8emul::cpu::reg::V0, 0xAA);
    CHECK(cpu.get_register(yac8emul::cpu::reg::V0) == 0xAA);
}

TEST_CASE("RAM initialization") {
  using namespace yac8emul;
  yac8emul::cpu cpu;
  CHECK(cpu.RAM[0] == constants::sprites[0]);
  CHECK(cpu.RAM[0x30] == constants::sprites[0x30]);
}
