#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mc8.h"

#define ADDRESS_COUNT 256
#define ROM_SIZE 32

namespace bus
{
    /*
        Memory Map
        0x00:        Emulator Halt (Write-Only)
        0x00 - 0x1F: ROM (Read-Only)
        0x20 - 0xFF: RAM (Read-Write)
        0xFE:        Serial Control (Read-Only)
        0xFF:        Serial Data (Read-Write)
    */

    uint8_t memory[ADDRESS_COUNT];

    enum BusError
    {
        NO_ERROR,
        OPEN_ROM_FILE_FAILURE,
        ROM_FILE_TOO_SMALL,
        CLOSE_ROM_FILE_FAILURE,
    };

    BusError init(const char *const rom_name)
    {
        for (size_t idx = 0; idx < ADDRESS_COUNT; idx++)
            memory[idx] = 0;

        FILE *rom_file = fopen(rom_name, "r");
        if (rom_file == NULL)
            return OPEN_ROM_FILE_FAILURE;

        if (fread(memory, sizeof(uint8_t), ROM_SIZE, rom_file) != ROM_SIZE)
            return ROM_FILE_TOO_SMALL;

        if (fclose(rom_file) != 0)
            return CLOSE_ROM_FILE_FAILURE;

        return NO_ERROR;
    }

    uint8_t read(uint8_t addr)
    {
        return memory[addr];
    }

    bool write(uint8_t addr, uint8_t data)
    {
        if (addr == 0x00) {
            printf("Emulator halting (store to 0x00)\n");
            return false;
        }

        if (addr > 0x1f)
            memory[addr] = data;

        if (addr == 0xff)
            printf("%c", (char) data);

        return true;
    }
};

int main(int argc, char *argv[])
{
    const char* rom_name;
    size_t max_inst_cycles = 100;

    if (argc < 2) {
        printf("Provide path to ROM file.\n");
        exit(-1);
    }

    if (argc > 2) {
        max_inst_cycles = atoi(argv[2]);
    }

    rom_name = argv[1];
    bus::BusError error = bus::init(rom_name);
    if (error != bus::NO_ERROR) {
        printf("Bus initialization failed with code %d\n", error);
        exit(-1);
    }

    mc8::mc8 *cpu = mc8::create(&bus::read, &bus::write);

    printf("%s", "Emulation beginning.\n\n");

    size_t actual_cycles = mc8::run(cpu, max_inst_cycles);
    mc8::release(cpu);

    return 0;
}
