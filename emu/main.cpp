#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mc8.h"
#include "dis.h"
#include "bus.h"

typedef struct {
    char* rom_name;
    int max_cycles;
} ParsedArguments;

void usage(const char* const program_name) {
    fprintf(
        stderr,
        "usage: %s rom_file [ncycles]\n"
        "    rom_file: input rom to initialize bus with\n"
        "    ncycles: maximum number of instruction cycles to execute\n"
        "             (default is 100)\n",
        program_name
    );
}

ParsedArguments parse_arguments(const int argc, char* const argv[]) {
    ParsedArguments args;
    char* const program_name = argv[0];

    if (argc < 2) {
        fprintf(stderr,"Provide path to ROM file.\nSee --help for usage details.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "--help") == 0) {
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    args.rom_name = argv[1];
    args.max_cycles = 1000;

    if (argc == 2) {
        return args;
    }

    args.max_cycles = atoi(argv[2]);
    if (!args.max_cycles && strcmp(argv[2], "0")) {
        fprintf(
            stderr,
            "Optional argument ncycles must be integer (got \"%s\").\n"
            "See --help for usage details.\n",
            argv[2]
        );
        exit(EXIT_FAILURE);
    }

    if (argc > 3) {
        fprintf(stderr, "Arguments beginning with \"%s\" are invalid.\nSee --help for usage details.\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    return args;
}

int main(int argc, char *argv[]) {
    const ParsedArguments args = parse_arguments(argc, argv);
    printf(
        "Parsed Arguments:\n"
        "ROM Name: %s\n"
        "Max Cycles: %d\n\n",
        args.rom_name,
        args.max_cycles
    );

    bus::BusError bus_error;
    bus::Bus *bus = bus::create(args.rom_name, &bus_error);
    if (bus_error != bus::NO_ERROR) {
        printf("Bus initialization failed with code %s\n", bus::error_repr(bus_error));
        return EXIT_FAILURE;
    }

    printf("Disassembly of ROM (%d bytes)\n", bus::ROM_SIZE);
    bus::disassemble(bus, stdout, bus::ROM_BASE, bus::ROM_BASE + bus::ROM_SIZE);
    printf("\n");

    mc8::CpuError cpu_error;
    mc8::mc8 *cpu = mc8::create(bus, &cpu_error);
    if (cpu_error != mc8::NO_ERROR) {
        printf("CPU initialization failed with code %s\n", mc8::error_repr(cpu_error));
        return EXIT_FAILURE;
    }

    printf("%s", "Emulation beginning.\n\n");
    uint32_t actual_cycles = mc8::run(cpu, args.max_cycles);

    printf("\nEmulation completed.\n");
    printf("Executed %d instructions.\n", actual_cycles);

    mc8::release(cpu);
    return EXIT_SUCCESS;
}
