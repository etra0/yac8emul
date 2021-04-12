#pragma once

#include <yac8emul/yac8emul.h>
#include <yac8emul/constants.h>


namespace yac8emul {
    class cpu {
    public:
        enum class reg: std::uint8_t {
            V0 = 0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF
        };

        cpu() : pc(0), RAM(), registers(), sp(0), stack(), frame_buffer() {
            this->registers.fill(0);
            this->RAM.fill(0);
            this->stack.fill(0);
            this->frame_buffer.fill(0);

            // copy the sprites to the beginning of the RAM
            using namespace yac8emul::constants;
            std::copy(sprites.begin(), sprites.end(), this->RAM.begin());
        };

        void change_register(cpu::reg r, std::uint8_t val) noexcept;

        std::uint8_t get_register(cpu::reg r) noexcept;

        std::array<std::uint8_t, 4096> RAM;
    private:
        std::array<std::uint8_t, 16> registers;
        std::array<std::uint16_t, 16> stack;
        std::array<bool, 64 * 32> frame_buffer;

        // Special registers
        // Program Counter
        std::uint16_t pc;

        // Stack Pointer
        std::uint8_t sp;

    };

    int sum(int a, int b);
}
