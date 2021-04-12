#include <yac8emul/interpreter.h>

namespace yac8emul {

void cpu::change_register(cpu::reg r, std::uint8_t val) noexcept {
    this->registers[static_cast<size_t>(r)] = val;
}

uint8_t cpu::get_register(cpu::reg r) noexcept { return this->registers[static_cast<size_t>(r)]; }
}

