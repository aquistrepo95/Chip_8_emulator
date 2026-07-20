#include <iostream>
#include "SDL3_platform.hpp"


bool SDL3_platform :: init() {
    // initialize SDL3
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cerr << "SDL3 Init Failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // create window
    window = SDL_CreateWindow("CHIP-8 Emulator", CHIP8_WIDTH * SCALE, CHIP8_HEIGHT * SCALE, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
    if(!window) {
        std::cerr << "Window Creation Failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // create renderer
    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer) {
        std::cerr << "Renderer Creation Failed: " << SDL_GetError() << std::endl;
        return false;
    }
    /*
    SDL_SetRenderLogicalPresentation(
        renderer, 
        CHIP8_WIDTH, 
        CHIP8_HEIGHT, 
        SDL_LOGICAL_PRESENTATION_INTEGER_SCALE
    );*/

    if(!SDL_SetRenderVSync(renderer, 1)) {
        return false;
    }

    // create texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, CHIP8_WIDTH, CHIP8_HEIGHT);
    if(!texture) {
        std::cerr << "Texture Creation Failed: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    clear();
    return true;
}

// destructor
SDL3_platform :: ~SDL3_platform() {
    if(texture) {
        SDL_DestroyTexture(texture);
    }
    if(renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if(window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void SDL3_platform :: clear() {
    // clear the pixel buffer
    for(size_t i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; ++i) {
        pixelBuffer[i] = 0x00000000; // set to black
    }
}

void SDL3_platform :: render(const uint32_t* displaychip8) {
    // map the CHIP-8 display to the pixel buffer
    for(size_t i = 0; i < CHIP8_WIDTH * CHIP8_HEIGHT; ++i) {
        pixelBuffer[i] = displaychip8[i] ? 0xFFFFFFFF : 0x00000000; // white or black
    }

    // update the texture with the pixel buffer
    SDL_UpdateTexture(texture, nullptr, pixelBuffer, CHIP8_WIDTH * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void handle_sdl_events(uint8_t* keypad, bool& running) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {

        if (event.type == SDL_EVENT_QUIT) {
            running = false;
            return;
        }

        if(event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
            bool isKeyDown = (event.type == SDL_EVENT_KEY_DOWN);

            switch(event.key.key) {
                case SDLK_X: keypad[0] = isKeyDown; break;
                case SDLK_1: keypad[1] = isKeyDown; break;
                case SDLK_2: keypad[2] = isKeyDown; break;
                case SDLK_3: keypad[3] = isKeyDown; break;

                case SDLK_Q: keypad[4] = isKeyDown; break;
                case SDLK_W: keypad[5] = isKeyDown; break;
                case SDLK_E: keypad[6] = isKeyDown; break;
                case SDLK_A: keypad[7] = isKeyDown; break;

                case SDLK_S: keypad[8] = isKeyDown; break;
                case SDLK_D: keypad[9] = isKeyDown; break;
                case SDLK_Z: keypad[0xA] = isKeyDown; break;
                case SDLK_C: keypad[0xB] = isKeyDown; break;

                case SDLK_4: keypad[0xC] = isKeyDown; break;
                case SDLK_R: keypad[0xD] = isKeyDown; break;
                case SDLK_F: keypad[0xE] = isKeyDown; break;
                case SDLK_V: keypad[0xF] = isKeyDown; break;

                case SDLK_ESCAPE: running = false; break;
                default: break;
            }
        }
    }

}