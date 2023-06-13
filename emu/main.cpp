#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mc8.h"

#define ADDRESS_COUNT 256
#define ROM_SIZE 32

namespace bus {
    /*
        Memory Map
        0x00:        Halt Emulator (Write-Only)
        0x00 - 0x1d: ROM (Read-Only)
        0x1e:        UART Control (Read-Only)
        0x1f:        UART Data (Read-Write)
        0x20 - 0xff: RAM (Read-Write)
    */

    uint8_t memory[ADDRESS_COUNT];

    enum BusError {
        NO_ERROR,
        OPEN_ROM_FILE_FAILURE,
        ROM_FILE_TOO_SMALL,
        CLOSE_ROM_FILE_FAILURE,
    };

    BusError init(const char* const rom_name) {
        for (size_t idx = 0; idx < ADDRESS_COUNT; idx++)
            memory[idx] = 0;

        FILE* rom_file = fopen(rom_name, "r");
        if (rom_file == NULL)
            return OPEN_ROM_FILE_FAILURE;

        if (fread(memory, sizeof(uint8_t), ROM_SIZE, rom_file) != ROM_SIZE)
            return ROM_FILE_TOO_SMALL;
        
        if (fclose(rom_file) != 0)
            return CLOSE_ROM_FILE_FAILURE;

        return NO_ERROR;
    }

    uint8_t read(uint8_t addr) {
        return memory[addr];
    }

    bool write(uint8_t addr, uint8_t data) {
        if (addr == 0x00) {
            printf("Writing to zero.\n");
            return false;
        }

        if (addr == 0x1f) {
            printf("%c", data);
        } else if (addr > 0x1e) {
            memory[addr] = data;
        }

        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Provide path to ROM file.\n");
        exit(-1);
    }

    const char* rom_name = argv[1];
    bus::BusError error = bus::init(rom_name);
    if (error != bus::NO_ERROR) {
        printf("Bus initialization failed with code %d\n", error);
        exit(-1);
    }

    mc8::mc8* cpu = mc8::create(&bus::read, &bus::write);
    size_t cycles = mc8::run(cpu);
    mc8::release(cpu);

    return 0;
}
