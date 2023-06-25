#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "mc8.h"
#include "dis.h"
#include "bus.h"

int parse_arguments(
    int argc,
    char* argv[],
    const char** rom_name,
    int* max_inst_cycles,
    bool* show_disassembly
) {
    *show_disassembly = false;
    *max_inst_cycles = 100;

    if (argc < 2) {
        return -1;
    }

    *rom_name = argv[1];

    if (argc > 2) {
        *max_inst_cycles = atoi(argv[2]);
    }

    if (argc > 3) {
        if (strcmp(argv[3], "dis") == 0) {
            *show_disassembly = true;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    const char* rom_name;
    int max_inst_cycles = 100;
    bool show_disassembly = false;

    if (
        parse_arguments(
            argc,
            argv,
            &rom_name,
            &max_inst_cycles,
            &show_disassembly
        ) < 0
    ) {
        printf("Provide path to ROM file.\n");
        return -1;
    }

    printf("Parsed arguments:\n");
    printf("- ROM Name: %s\n", rom_name);
    printf("- Max Instruction Cycles: %d\n", max_inst_cycles);
    printf("- Show Disassembly: %s\n\n", show_disassembly ? "true" : "false");

    bus::BusError error = bus::init(rom_name);
    if (error != bus::NO_ERROR) {
        printf(
            "Bus initialization failed with code %s\n",
            bus::error_repr(error)
        );
        return -1;
    }

    if (show_disassembly) {
        printf("Disassembly of ROM (%d bytes)\n", bus::ROM_SIZE);
        bus::disassemble(stdout, bus::ROM_BASE, bus::ROM_BASE + bus::ROM_SIZE);
        printf("\n");
    }

    mc8::mc8 *cpu = mc8::create(&bus::read, &bus::write);
    if (cpu == NULL) {
        printf("Failed to allocate memory for CPU.\n");
        return -1;
    }

    printf("%s", "Emulation beginning.\n\n");

    uint32_t actual_cycles = mc8::run(cpu, max_inst_cycles);
    printf("\nEmulation completed.\n");
    printf("Executed %d instructions.\n", actual_cycles);

    mc8::release(cpu);

    return 0;
}
