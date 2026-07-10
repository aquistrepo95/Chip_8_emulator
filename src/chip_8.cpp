#include <iostream>
#include <functional>
#include <array>
#include <cstdint>
#include <random>
#include <fstream>
#include <vector>
#include "chip_8.hpp"


// constructor
chip_8 :: chip_8() {
    // initialize memory
    program_counter = PROGRAM_START_ADDRESS;

    // load fontset into memory
    for(unsigned int i = 0; i < FONTSET_SIZE; ++i) {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    // initialize random number generator
    random_engine.seed(std::random_device{}());
    distribution = std::uniform_int_distribution<uint16_t>(0, 255U);
}

// chip cycle
void chip_8 :: chip_cycle() {
    // fetch 16-bit opcode from memory
    opcode = (memory[program_counter] << 8) | memory[program_counter + 1];

    std::cout << "Opcode: " << std::hex << opcode << std::endl;

    // increment program counter
    program_counter += 2;

    // decode and execute opcode
    std::invoke(MainTable[opcode >> 12], this);

    // update timers
    if(delay_timer > 0) {
        --delay_timer;
    }

    if(sound_timer > 0) {
        --sound_timer;
    }

}

// load ROM
void chip_8 :: load_ROM(const char* RomFile) {
    // open ROM file and move pointer to the end to get size
    std::ifstream file(RomFile, std::ios::binary | std::ios::ate);

    // verify if file is open and print error message if not or load ROM into memory
    if(!file.is_open()) {
        std::cerr << "Failed to open ROM file: " << RomFile << std::endl;
        return;
    }

    else {
        // get size of file and allocate buffer to hold contents
        std::streamsize file_size = file.tellg();
        std::vector<char> buffer(file_size);

        // move pointer back to the beginning of the file and read the contents into the buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), file_size);
        file.close();

        // load ROM into memory starting at 0x200
        for(std::streamsize i = 0; i < file_size; ++i) {
            memory[PROGRAM_START_ADDRESS + i] = static_cast<std::uint8_t>(buffer[i]);
        }

    }
}

// handle 0x0 opcodes
void chip_8 :: handle_0x0() {
    // invoke the appropriate function from the subtable based on the last nibble of the opcode (0x000n)
    std::invoke(SubTable0[opcode & 0x000F], this);
}

// handle 0x8 opcodes
void chip_8 :: handle_0x8() {
    // invoke the appropriate function from the subtable based on the last nibble of the opcode (0x000n)
    std::invoke(SubTable8[opcode & 0x000F], this);
}

// handle 0xE opcodes
void chip_8 :: handle_0xE() {
    // invoke the appropriate function from the subtable based on the last nibble of the opcode (0x000n)
    std::invoke(subtableE[opcode & 0x000F], this);
}

// handle 0xF opcodes
void chip_8 :: handle_0xF() {
    // invoke the appropriate function from the subtable based on the last two nibbles of the opcode (0x00nn)
    std::invoke(subtableF[opcode & 0x00FF], this);
}


// opcode functions
void chip_8 :: opcode_00E0() {
    std::ranges::fill(display, 0);

}

void chip_8 :: opcode_00EE() {
    program_counter = stack[--stack_pointer];
}

void chip_8 :: opcode_1NNN() {
    uint16_t target_address = opcode & 0x0FFFu;

    program_counter = target_address;
}

void chip_8 :: opcode_2NNN() {
    uint16_t target_address = opcode & 0x0FFFu;

    stack[stack_pointer] = program_counter;
    ++stack_pointer;
    program_counter = target_address;
}

void chip_8 :: opcode_3xkk() {
    uint8_t kk = opcode & 0x00FFu;
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    if(registers[Vx] == kk) {
        program_counter += 2;
    }
}

void chip_8 :: opcode_4xkk() {
    uint8_t kk = opcode & 0x00FFu;
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    if(registers[Vx] != kk) {
        program_counter += 2;
    }
}

void chip_8 :: opcode_5xy0() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] == registers[Vy]) {
        program_counter += 2;
    }
}

void chip_8 :: opcode_6xkk() {
    uint8_t kk = opcode & 0x00FFu;
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = kk;
}

void chip_8 :: opcode_7xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t kk = opcode & 0x00FFu;

    registers[Vx] += kk;
}

void chip_8 :: opcode_8xy0() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = registers[Vy];
}

void chip_8 :: opcode_8xy1() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];
}

void chip_8 :: opcode_8xy2() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];
}

void chip_8 :: opcode_8xy3() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];
}

void chip_8 :: opcode_8xy4() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = registers[Vx] + registers[Vy];

    registers[0xF] = (sum > 255) ? 1 : 0;

    registers[Vx] = sum & 0xFF;
}

void chip_8 :: opcode_8xy5() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[Vx] > registers[Vy]) ? 1 : 0;

    registers[Vx] -= registers[Vy];
}

void chip_8 :: opcode_8xy6() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = registers[Vx] & 0x1;

    registers[Vx] >>= 1;
}

void chip_8 :: opcode_8xy7() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[Vy] > registers[Vx]) ? 1 : 0;

    registers[Vx] = registers[Vy] - registers[Vx];
}

void chip_8 :: opcode_8xyE() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = (registers[Vx] & 0x80) >> 7;

    registers[Vx] <<= 1;
}

void chip_8 :: opcode_9xy0() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] != registers[Vy]) {
        program_counter += 2;
    }
}

void chip_8 :: opcode_Annn() {
    uint16_t target_address = opcode & 0x0FFFu;

    index_register = target_address;
}

void chip_8 :: opcode_Bnnn() {
    uint16_t target_address = opcode & 0x0FFFu;

    program_counter = target_address + registers[0];
}

void chip_8 :: opcode_Cxkk() {
    uint8_t kk = opcode & 0x00FFu;
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = static_cast<uint8_t>(distribution(random_engine)) & kk;
}

void chip_8 :: opcode_Dxyn() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    // calculate the starting coordinates for drawing
    uint8_t x = registers[Vx] % DISPLAY_WIDTH;
    uint8_t y = registers[Vy] % DISPLAY_HEIGHT;

    // reset the collision flag
    registers[0xF] = 0;

    // draw the sprite row by row
    for(unsigned int row = 0; row < height; ++row) {
        // get the sprite byte from memory
        uint8_t sprite_byte = memory[index_register + row];

        // draw the sprite pixel by pixel
        for(unsigned int col = 0; col < 8; ++col) {

            uint8_t sprite_pixel = sprite_byte & (0x80 >> col);
            uint32_t* display_index = &display[(y + row) * DISPLAY_WIDTH + (x + col)];  

            if(sprite_pixel) {
                if(*display_index == 0xFFFFFFFF) {
                    registers[0xF] = 1;
                }
                *display_index ^= 0xFFFFFFFF;
            }     
        }
    }     
}

void chip_8 :: opcode_Ex9E() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    uint8_t key = registers[Vx];

    if(keypad[key]) {
        program_counter += 2;
    }

}

void chip_8 :: opcode_ExA1() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    uint8_t key = registers[Vx];

    if(!keypad[key]) {
        program_counter += 2;
    }
}

void chip_8 :: opcode_Fx07() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = delay_timer;
}

void chip_8 :: opcode_Fx0A() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    if(keypad[0]) {
        registers[Vx] = 0;
    }
    else if(keypad[1]) {
        registers[Vx] = 1;
    }
    else if(keypad[2]) {
        registers[Vx] = 2;
    }
    else if(keypad[3]) {
        registers[Vx] = 3;
    }
    else if(keypad[4]) {
        registers[Vx] = 4;
    }
    else if(keypad[5]) {
        registers[Vx] = 5;
    }
    else if(keypad[6]) {
        registers[Vx] = 6;
    }
    else if(keypad[7]) {
        registers[Vx] = 7;
    }
    else if(keypad[8]) {
        registers[Vx] = 8;
    }
    else if(keypad[9]) {
        registers[Vx] = 9;
    }
    else if(keypad[10]) {
        registers[Vx] = 10;
    }
    else if(keypad[11]) {
        registers[Vx] = 11;
    }
    else if(keypad[12]) {
        registers[Vx] = 12;
    }
    else if(keypad[13]) {
        registers[Vx] = 13;
    }
    else if(keypad[14]) {
        registers[Vx] = 14;
    }
    else if(keypad[15]) {
        registers[Vx] = 15;
    }
    else {
        program_counter -= 2; // wait for a key press
    }
}

void chip_8 :: opcode_Fx15() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    delay_timer = registers[Vx];
}

void chip_8 :: opcode_Fx18() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    sound_timer = registers[Vx];
}

void chip_8 :: opcode_Fx1E() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    index_register += registers[Vx];
}

void chip_8 :: opcode_Fx29() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vx_digit = registers[Vx];

    index_register = FONTSET_START_ADDRESS + (Vx_digit * 5);
}

void chip_8 :: opcode_Fx33() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vx_value = registers[Vx];

    memory[index_register]     = Vx_value / 100;
    memory[index_register + 1] = (Vx_value / 10) % 10;
    memory[index_register + 2] = Vx_value % 10;
}

void chip_8 :: opcode_Fx55() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(uint8_t i = 0; i <= Vx; ++i) {
        memory[index_register + i] = registers[i];
    }
}

void chip_8 :: opcode_Fx65() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    for(uint8_t i = 0; i <= Vx; ++i) {
        registers[i] = memory[index_register + i];
    }
}

void chip_8 :: opcode_unknown() {

    std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
}