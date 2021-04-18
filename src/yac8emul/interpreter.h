#pragma once

#include <yac8emul/yac8emul.h>
#include <yac8emul/constants.h>
#include <yac8emul/errors.h>


namespace yac8emul {
    class cpu {
    public:
        enum class reg: std::uint8_t {
            V0 = 0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF
        };

        enum class instruction : std::uint8_t {
            // SYS addr, CLS or RET
            SYSTEM = 0x0,
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
            // sprite is positioned so part of it is outside the coordinates of the display, 
            // it wraps around to the opposite side of the screen.
            DRW,
            // Ex9E - SKP Vx -  Skip next instruction if key with the value of Vx is pressed.
            // ExA1 - SKNP Vx - Skip next instruction if key with the value of Vx is not pressed.
            SKP,
            // 0xFxkk - Special LD, where kk:
            // 07 - Set Vx = delay timer
            // 0A - Wait for a key press, store in Vx (execution should be paused).
            // 15 - Set Delay timer = Vx.
            // 18 - Sound timer = Vx.
            // 1E - I = I + Vx.
            // 29 - Set I = location of sprite for digit Vx. (Load the sprite addr?)
            // 33 - Store BCD representation of Vx in memory locations I, I+1, and I+2.
            // 55 - Store register V0 to Vx in memory starting at I.,
            // 65 - Read registers through V0 to Vx starting at location I.
            SPECLD,
        };

        // TODO: Check if PC *really* starts at 0x200.
        cpu() : pc(0x200), RAM(), registers(), sp(0), stack(), frame_buffer(), i(0), random_state(1234), dt(0), st(0) {
            this->registers.fill(0);
            this->RAM.fill(0);
            this->disp_clear();

            // copy the sprites to the beginning of the RAM
            using namespace yac8emul::constants;
            std::copy(sprites.begin(), sprites.end(), this->RAM.begin());
        };

        void disp_clear();
        void parse_instruction(std::uint16_t inst);
        void execute_instruction();
        void execute_regop(cpu::reg Vx, cpu::reg Vy, std::uint8_t modifier);
        void execute_specld(cpu::reg Vx, std::uint8_t modifier);
        void load_rom(const std::vector<std::uint8_t>& rom);
        const std::array<std::uint8_t, 4096>& get_ram() noexcept;

        // Fast Xorshift because I think we don't need anything better.
        std::uint8_t get_random_value();

        void run();
        std::uint8_t& get_register(cpu::reg r) noexcept;
        
    private:
        std::array<std::uint8_t, 16> registers;
        std::stack<std::uint16_t> stack;
        std::array<std::array<bool, 32>, 64> frame_buffer;
        std::array<std::uint8_t, 4096> RAM;

        // Special registers
        // Program Counter
        std::uint16_t pc;

        // Stack Pointer
        std::uint8_t sp;

        // Special register to store an address
        std::uint16_t i;

        // Delay timer
        std::uint8_t dt;

        // Sound timer
        std::uint8_t st;

        std::uint64_t random_state;

    };

    int sum(int a, int b);
}
