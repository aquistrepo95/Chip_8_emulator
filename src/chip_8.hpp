#ifndef CHIP_8_H
#define CHIP_8_H
#include <array>
#include <cstdint>
#include <random>

// constant variable for chip 8 specifications
const unsigned int MEMORY_SIZE      = 4096;
const unsigned int REGISTER_COUNT   = 16;
const unsigned int STACK_SIZE       = 16;
const unsigned int KEYPAD_SIZE      = 16;  
const unsigned int DISPLAY_WIDTH    = 64;
const unsigned int DISPLAY_HEIGHT   = 32; 
const unsigned int FONTSET_SIZE     = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int PROGRAM_START_ADDRESS = 0x200;

class chip_8 {
    public:
        // constructor
        chip_8();

        // start the emulation
        void chip_cycle();

        // load the ROM file into memory
        void load_ROM(const char*);

        // define alias for array pointer
        using Chip8Function = void (chip_8::*)();

        // define main table for opcodes
        using Chip8MainTable = std::array<Chip8Function, 0xF + 1>;

        // define subtable for opcodes
        using Chip8SubTable = std::array<Chip8Function,   0xE + 1>;
        using Chip8SubTableF = std::array<Chip8Function,  0x65 + 1>;

        // handle functions for opcodes that repeat
        void handle_0x0();
        void handle_0x8();
        void handle_0xE();
        void handle_0xF();


        // define keypad
        std::uint8_t keypad[KEYPAD_SIZE]{};

        // define display
        std::uint32_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT]{};


        // define array for fontset
        using FontsetArray = std::array<std::uint8_t, FONTSET_SIZE>;

        // fontset
        static inline constexpr FontsetArray fontset = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		    0x20, 0x60, 0x20, 0x20, 0x70, // 1
		    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };   


    private:
        // specifications
        std::uint8_t memory[MEMORY_SIZE]{};
        std::uint8_t registers[REGISTER_COUNT]{};
        std::uint16_t index_register{};
        std::uint16_t program_counter{};
        std::uint16_t stack[STACK_SIZE]{};
        std::uint8_t stack_pointer{};
        std::uint8_t delay_timer{};
        std::uint8_t sound_timer{};
    
        // opcode 
        std::uint16_t opcode{};


        // opcode functions
        void opcode_00E0();
        void opcode_00EE();
        void opcode_1NNN();
        void opcode_2NNN();
        void opcode_3xkk();
        void opcode_4xkk();
        void opcode_5xy0();
        void opcode_6xkk();
        void opcode_7xkk();
        void opcode_8xy0();
        void opcode_8xy1();
        void opcode_8xy2();
        void opcode_8xy3();
        void opcode_8xy4();
        void opcode_8xy5();
        void opcode_8xy6();
        void opcode_8xy7();
        void opcode_8xyE();
        void opcode_9xy0();
        void opcode_Annn();
        void opcode_Bnnn();
        void opcode_Cxkk();
        void opcode_Dxyn();
        void opcode_Ex9E();
        void opcode_ExA1();
        void opcode_Fx07();
        void opcode_Fx0A();
        void opcode_Fx15();
        void opcode_Fx18();
        void opcode_Fx1E();
        void opcode_Fx29();
        void opcode_Fx33();
        void opcode_Fx55();
        void opcode_Fx65();
        void opcode_unknown();


        // opcode main table
        static inline constexpr Chip8MainTable MainTable = []() {
            Chip8MainTable table{};
            table[0x0] = &chip_8::handle_0x0;
            table[0x1] = &chip_8::opcode_1NNN;
            table[0x2] = &chip_8::opcode_2NNN;
            table[0x3] = &chip_8::opcode_3xkk;
            table[0x4] = &chip_8::opcode_4xkk;
            table[0x5] = &chip_8::opcode_5xy0;
            table[0x6] = &chip_8::opcode_6xkk;
            table[0x7] = &chip_8::opcode_7xkk;
            table[0x8] = &chip_8::handle_0x8;
            table[0x9] = &chip_8::opcode_9xy0;
            table[0xA] = &chip_8::opcode_Annn;
            table[0xB] = &chip_8::opcode_Bnnn;
            table[0xC] = &chip_8::opcode_Cxkk;
            table[0xD] = &chip_8::opcode_Dxyn;
            table[0xE] = &chip_8::handle_0xE;
            table[0xF] = &chip_8::handle_0xF;
    
            return table;
        }();
        
        // opcode subtables for 0x0 0x8, 0xE, 0xF
        static inline constexpr Chip8SubTable SubTable0 = []() {
            Chip8SubTable table{};
            table.fill(&chip_8::opcode_unknown);
            table[0x0] = &chip_8::opcode_00E0;
            table[0xE] = &chip_8::opcode_00EE;

            return table;
        }();

        static inline constexpr Chip8SubTable SubTable8 = []() {
            Chip8SubTable table{};
            table.fill(&chip_8::opcode_unknown);
            table[0x0] = &chip_8::opcode_8xy0;
            table[0x1] = &chip_8::opcode_8xy1;
            table[0x2] = &chip_8::opcode_8xy2;
            table[0x3] = &chip_8::opcode_8xy3;
            table[0x4] = &chip_8::opcode_8xy4;
            table[0x5] = &chip_8::opcode_8xy5;
            table[0x6] = &chip_8::opcode_8xy6;
            table[0x7] = &chip_8::opcode_8xy7;
            table[0xE] = &chip_8::opcode_8xyE;

            return table;
        }();
        

        static inline constexpr Chip8SubTable subtableE = []() {   
            Chip8SubTable table{};
            table.fill(&chip_8::opcode_unknown);
            table[0x1] = &chip_8::opcode_ExA1;
            table[0xE] = &chip_8::opcode_Ex9E;
    
            return table;
        }();
        

        static inline constexpr Chip8SubTableF subtableF = []() {
            Chip8SubTableF table{};
            table.fill(&chip_8::opcode_unknown);
            table[0x07] = &chip_8::opcode_Fx07;
            table[0x0A] = &chip_8::opcode_Fx0A;
            table[0x15] = &chip_8::opcode_Fx15;
            table[0x18] = &chip_8::opcode_Fx18;
            table[0x1E] = &chip_8::opcode_Fx1E;
            table[0x29] = &chip_8::opcode_Fx29;
            table[0x33] = &chip_8::opcode_Fx33;
            table[0x55] = &chip_8::opcode_Fx55;
            table[0x65] = &chip_8::opcode_Fx65;

            return table;
        }();       

        // add random number generator for instruction Cxkk  
        std::mt19937 random_engine;
        std::uniform_int_distribution<uint16_t> distribution;
};


#endif // CHIP_8_H