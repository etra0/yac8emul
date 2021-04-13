#pragma once

#include <yac8emul/yac8emul.h>
#include <yac8emul/constants.h>


namespace yac8emul {
    class cpu {
    public:
        enum class reg: std::uint8_t {
            V0 = 0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF
        };

        enum class instruction : std::uint8_t {
            // SYS addr, CLS or RET
            SYSTEM =  0x0,
            // JMP addr - (1nnn)
            JP,
            // Call addr - (2nnn)
            CALL,
            // Skip next instruction if Vx == imm (kk) - (3xkk)
            SEimm,
            // Skip next instruction if Vx != imm (kk) - (4xkk)
            SNEimm,
            // Skip next instruction if Vx == Vy - (5xy0)
            SE,
            // Load the imm into x - (6xkk)
            LDimm,
            // Adds kk to Vx - (7xkk)
            ADD,
            // Loads y to x - (8xy0)
            LD,
            // 2 register operations, (8xyK)
            // where K:
            // 0 -> Vk = Vy
            // 1 -> Vx = Vx OR Vy
            // 2 -> Vx = Vx AND Vy
            // 3 -> Vx = Vx XOR Vy
            // 4 -> Vx += Vy, If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
            // 5 -> Vx = Vx - Vy, If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
            // 6 -> Vx >>= 1, if LSB is 1, VF is set to 1.
            // 7 -> Vx = Vy - Vx, If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
            // E -> Vx <<= 1. If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
            REGOP,
            // Skip next instruction if Vx != Vy - (9xy0)
            SNE,
            // Load nnn into I - (Annn)
            LDI,
            // Jump into V0 + nnn - (Bnnn)
            JPV0,
            // Vx = RND And kk - (Cxkk)
            RND,
            // Read n bytes from I, then displayed at Vx, Vy. Sprites are XORed onto the existing screen. 
            // If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the 
            // sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
            DRW,
            // Ex9E - SKP Vx -  Skip next instruction if key with the value of Vx is pressed.
            // ExA1 - SKNP Vx - Skip next instruction if key with the value of Vx is not pressed.
            SKP
        };

        // TODO: Check if PC *really* starts at 0x200.
        cpu() : pc(0x200), RAM(), registers(), sp(0), stack(), frame_buffer() {
            this->registers.fill(0);
            this->RAM.fill(0);
            this->frame_buffer.fill(0);


            // copy the sprites to the beginning of the RAM
            using namespace yac8emul::constants;
            std::copy(sprites.begin(), sprites.end(), this->RAM.begin());
        };

        void set_register(cpu::reg r, std::uint8_t val) noexcept;
        void parse_instruction(std::uint16_t inst);
        void load_rom(const std::vector<std::uint8_t>& rom);
        void run();

        std::uint8_t get_register(cpu::reg r) noexcept;

        std::array<std::uint8_t, 4096> RAM;
    private:
        std::array<std::uint8_t, 16> registers;
        std::stack<std::uint16_t> stack;
        std::array<bool, 64 * 32> frame_buffer;

        // Special registers
        // Program Counter
        std::uint16_t pc;

        // Stack Pointer
        std::uint8_t sp;

        // Special register to store an address
        std::uint16_t i;

    };

    int sum(int a, int b);
}
