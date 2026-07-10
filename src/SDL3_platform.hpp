#ifndef SDL3_PLATFORM_HPP
#define SDL3_PLATFORM_HPP
#include <SDL3/SDL.h>

class SDL3_platform {
    private:
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        SDL_Texture* texture{};

        static constexpr int CHIP8_WIDTH = 64;
        static constexpr int CHIP8_HEIGHT = 32;
        static constexpr int SCALE = 10; // scale factor for window size

        uint32_t pixelBuffer[CHIP8_WIDTH * CHIP8_HEIGHT];

    public:
        // Constructor
        SDL3_platform() = default;

        // Destructor
        ~SDL3_platform();

        // delete copy constructor and assignment operator
        SDL3_platform(const SDL3_platform&) = delete;
        SDL3_platform& operator=(const SDL3_platform&) = delete;

        // initialize SDL3
        bool init();

        // clear the pixel buffer
        void clear();

        // render the display
        void render(const uint32_t*); 

};

// handle SDL events and update the keypad state: declared here since chip_8 handles the keypad
void handle_sdl_events(uint8_t*keypad, bool& running);




#endif // SDL3_PLATFORM_HPP