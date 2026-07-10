#include <iostream>
#include <string>
#include "chip_8.hpp"
#include "SDL3_platform.hpp"

int main(int argc, char** argv) {
    // check if ROM file is provided
    if(argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <Delay>,<ROM file>" << std::endl;
        return 1;
    }

    int delay_cycles = std::stoi(argv[1]);
    std::string rom_file = argv[2];

    // create chip_8 instance
    chip_8 chip8;

    // load ROM file into chip_8 memory
    chip8.load_ROM(rom_file.c_str());

    // create SDL3 platform instance
    SDL3_platform sdl_platform;

    // initialize SDL3 platform
    if(!sdl_platform.init()) {
        return 1;
    }

    // main emulation loop
    bool running = true;

    while(running) {
        // handle SDL events and update keypad state
        handle_sdl_events(chip8.keypad, running);

        // execute chip cycle
        chip8.chip_cycle();

        // render the display
        sdl_platform.render(chip8.display);

        // delay for a short period to control emulation speed
        SDL_Delay(delay_cycles);

    }
    

    return 0;
}

