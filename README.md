# Chip_8_emulator

<img width="500" height="300" alt="Screenshot 2026-07-11 at 2 11 33 PM" src="https://github.com/user-attachments/assets/8f7b7460-1c40-4ad1-b1de-bfd3785105a5" />

## Chip 8 emulator/interpreter built with C++ 20 and SDL3

## Built With
* C++ 
* SDL3 for graphics processing.
* CMake for building.

## How to run the emulator
* Git clone the repo into a directory of your choice.
  ```
  git clone https://github.com/aquistrepo95/Chip_8_emulator.git
  ```
* Navigate to the root directory of the project, compile, and build
  ```
  cd chip_8
  cmake -S . -B build
  cmake --build build
  ```  
* Load and run ROMs.
  ```
  ./build/chip_8 3 ROMs/<select the ROM to run>
  ex: ./build/chip_8 3 ROMs/Tetris\ \[Fran\ Dachille,\ 1991\].ch8
  ```


<img width="500" height="300" alt="Screenshot 2026-07-11 at 2 11 51 PM" src="https://github.com/user-attachments/assets/0a73d15c-1ba3-4c31-b4e0-cf931f8f892e" />


