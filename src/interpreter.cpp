#include <yac8emul/interpreter.h>

namespace yac8emul {

void cpu::disp_clear() {
  for (auto &line : this->frame_buffer)
    line.fill(0);
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
    switch (inst) {
    case 0xE0:
      this->disp_clear();
      break;
    case 0xEE:
      this->pc = this->stack.top();
      this->stack.pop();
      break;
    default:
      throw yac8emul::errors::not_implemented("SYSCALL");
    }
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
  case cpu::instruction::LDimm: {
    std::uint8_t &v = this->get_register(Vx);
    v = nnn;
    break;
  }
  case cpu::instruction::ADD: {
    std::uint8_t &v = this->get_register(Vx);
    v += kk;
    break;
  }
  case cpu::instruction::REGOP:
    execute_regop(Vx, Vy, modifier);
    break;
  case cpu::instruction::SNE:
    if (this->get_register(Vx) != this->get_register(Vy))
      this->pc += 2;
    break;
  case cpu::instruction::LDI:
    this->i = nnn;
    break;
  case cpu::instruction::JPV0:
    this->pc = this->get_register(cpu::reg::V0) + nnn;
    break;
  case cpu::instruction::RND: {
    std::uint8_t &v = this->get_register(Vx);
    v = this->get_random_value() & kk;
    break;
  }
  case cpu::instruction::DRW:
    this->draw_on_screen(Vx, Vy, modifier);
    break;
  case cpu::instruction::SKP: {
    std::uint8_t &v = this->get_register(Vx);
    if (kk == 0x9E)
      this->pc += (v != 0) * 2;
    else if (kk == 0xA1)
      this->pc += (v == 0) * 2;
    break;
  }
  case cpu::instruction::SPECLD:
    this->execute_specld(Vx, kk);
    break;
  default:
    throw yac8emul::errors::invalid_instruction("main_instruction",
                                                std::to_string(inst));
  }
}

// TODO: Check the VF thing.
void cpu::draw_on_screen(cpu::reg Vx, cpu::reg Vy, std::uint8_t n) {
  std::uint8_t& pos_x = this->get_register(Vx);
  std::uint8_t& pos_y = this->get_register(Vy);
  for (std::size_t i = 0; i < n; i++) {
    this->frame_buffer[pos_x][pos_y + i] ^= this->i + i*sizeof(std::uint8_t);
  }
}

void cpu::execute_regop(cpu::reg Vx, cpu::reg Vy, std::uint8_t modifier) {
  std::uint8_t &reg_x = this->get_register(Vx);
  std::uint8_t &reg_y = this->get_register(Vy);
  std::uint8_t &reg_vf = this->get_register(cpu::reg::VF);
  std::uint16_t container = 0;

  switch (modifier) {
  case 0x0:
    reg_x = reg_y;
    break;
  case 0x1:
    reg_x |= reg_y;
    break;
  case 0x2:
    reg_x &= reg_y;
    break;
  case 0x3:
    reg_x ^= reg_y;
    break;
  case 0x4:
    container = reg_x + reg_y;
    reg_vf = container > 255;
    reg_x = container & 0xFF;
    break;
  case 0x5:
    // TODO: Check if this is correct.
    reg_vf = reg_x > reg_y;
    reg_x -= reg_y;
    break;
  case 0x6:
    reg_vf = reg_x & 1;
    reg_x >>= 1;
    break;
  case 0x7:
    reg_vf = reg_y > reg_x;
    reg_x = reg_y - reg_x;
    break;
  case 0xE:
    reg_vf = (0x80 & reg_x) >> 7;
    reg_x <<= 1;
    break;
  default:
    throw "illegal instruction";
  }
}

void cpu::execute_specld(cpu::reg Vx, std::uint8_t modifier) {
  std::uint8_t &reg_x = this->get_register(Vx);
  switch (modifier) {
  case 0x07:
    reg_x = this->dt;
    break;
  case 0x0A:
    throw yac8emul::errors::not_implemented("specld " + std::to_string(0x0A));
    break;
  case 0x15:
    this->dt = reg_x;
    break;
  case 0x18:
    this->st = reg_x;
    break;
  case 0x1E:
    this->i += reg_x;
    break;
  case 0x29:
    throw yac8emul::errors::not_implemented("specld " + std::to_string(0x29));
    break;
  case 0x33:
    throw yac8emul::errors::not_implemented("specld " + std::to_string(0x33));
    break;
  case 0x55:
    for (std::size_t i = 0; i <= static_cast<std::size_t>(Vx); i++) {
      auto ir = static_cast<cpu::reg>(i);
      this->RAM[this->i + i] = this->get_register(ir);
    }
    break;
  case 0x65:
    for (std::size_t i = 0; i <= static_cast<std::size_t>(Vx); i++) {
      auto &reg = this->get_register(static_cast<cpu::reg>(i));
      reg = this->RAM[this->i + i];
    }
    break;
  default:
    throw yac8emul::errors::invalid_instruction("modifier",
                                                std::to_string(modifier));
  }
}

void cpu::load_rom(const std::vector<uint8_t> &rom) {
  std::copy(rom.begin(), rom.end(), this->RAM.begin() + 0x200);
}

const std::array<std::uint8_t, 4096> &cpu::get_ram() noexcept {
  return this->RAM;
}

std::uint8_t cpu::get_random_value() {
  std::uint64_t x = this->random_state;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  this->random_state = x;
  return x & 0xFF;
}

void cpu::execute_instruction() {
  // Check if this should silently fail instead.
  if (this->pc > this->RAM.size() || this->pc + 1 > this->RAM.size())
    throw std::invalid_argument("EOL");

  this->parse_instruction(this->RAM[this->pc] << 8 |
      this->RAM[this->pc + 1]);
}

void cpu::run() {
  while (1) {
    if (this->pc > this->RAM.size() || this->pc + 1 > this->RAM.size())
      break;

    try {
      this->parse_instruction(this->RAM[this->pc] << 8 |
                              this->RAM[this->pc + 1]);
    } catch (yac8emul::errors::invalid_instruction &e) {
      std::cout << e.get_info() << std::endl;
      std::cout << "irrecuperable" << std::endl;
      break;

    } catch (yac8emul::errors::not_implemented &e) {
#ifdef YAC8EMUL_VERBOSE
      std::cout << e.get_info() << std::endl;
#endif
    }
    
  }
}

uint8_t &cpu::get_register(cpu::reg r) noexcept {
  return this->registers[static_cast<size_t>(r)];
}

} // namespace yac8emul
