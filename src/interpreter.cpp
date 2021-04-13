#include <yac8emul/interpreter.h>

namespace yac8emul {

void cpu::set_register(cpu::reg r, std::uint8_t val) noexcept {
  this->registers[static_cast<size_t>(r)] = val;
}

void cpu::parse_instruction(std::uint16_t inst) {
  cpu::instruction kind = static_cast<cpu::instruction>((inst & 0xF000) >> 12);
  cpu::reg Vx = static_cast<cpu::reg>((inst & 0x0F00) >> 8);
  cpu::reg Vy = static_cast<cpu::reg>((inst & 0x00F0) >> 4);
  std::uint8_t modifier = inst & 0xF;
  std::uint16_t nnn = inst & 0x0FFF;
  std::uint8_t kk = inst & 0xFF;

  this->pc += 2;
  switch (kind) {
  case cpu::instruction::SYSTEM:
    throw "SYSCALL";
    break;
  case cpu::instruction::JP:
    this->pc = nnn;
    break;
  case cpu::instruction::CALL:
    this->stack.push(this->pc);
    this->pc = nnn;
    break;
  case cpu::instruction::SEimm:
    if (this->get_register(Vx) == kk)
      this->pc += 2;
    break;
  case cpu::instruction::SNEimm:
    if (this->get_register(Vx) != kk)
      this->pc += 2;
    break;
  case cpu::instruction::SE:
    if (this->get_register(Vx) == this->get_register(Vy))
      this->pc += 2;
    break;
  case cpu::instruction::LDimm:
    this->set_register(Vx, nnn);
    break;
  default:
    throw "not implemented";
  }

}

void cpu::load_rom(const std::vector<uint8_t> &rom) {
  std::copy(rom.begin(), rom.end(), this->RAM.begin() + 0x200);
}

void cpu::run() {
  while (1) {
    this->parse_instruction(this->RAM[this->pc] << 8 | this->RAM[this->pc+1]);
  }
}

uint8_t cpu::get_register(cpu::reg r) noexcept {
  return this->registers[static_cast<size_t>(r)];
}

} // namespace yac8emul