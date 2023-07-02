#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "bus.h"
#include "dis.h"

namespace bus {
    /*
        Memory Map
        0x00:        Emulator Halt (Write-Only)
        0x00 - 0x1F: ROM (Read-Only)
        0x20 - 0xFF: RAM (Read-Write)
        0xFE:        Serial Control (Read-Only)
        0xFF:        Serial Data (Read-Write)
    */

    const uint16_t ADDRESS_SPACE_SIZE = 256;

    struct Bus {
        uint8_t memory[ADDRESS_SPACE_SIZE];
    };

    const uint8_t ROM_BASE = 0;
    const uint16_t ROM_SIZE = 32;

    const uint8_t SERIAL_OUTPUT = 0;
    const uint8_t HALT_EMULATOR = 1;
    const uint8_t SERIAL_CONTROL = 254;
    const uint8_t SERIAL_INPUT = 255;

    const char* error_repr(const BusError error) {
        switch (error) {
            case NO_ERROR:                  return "NO_ERROR";
            case BUS_ALLOCATION_FAILURE:    return "BUS_ALLOCATION_FAILURE";
            case OPEN_ROM_FILE_FAILURE:     return "OPEN_ROM_FILE_FAILURE";
            case ROM_FILE_TOO_SMALL:        return "ROM_FILE_TOO_SMALL";
            case CLOSE_ROM_FILE_FAILURE:    return "CLOSE_ROM_FILE_FAILURE";
            default:                        return "UNKNOWN_ERROR";
        }
    }

    Bus *create(char *const rom_name, BusError *const error) {
        *error = NO_ERROR;

        Bus *bus = (Bus*)malloc(sizeof(Bus));
        if (bus == NULL) {
            *error = BUS_ALLOCATION_FAILURE;
            return bus;
        }

        for (size_t idx = 0; idx < ADDRESS_SPACE_SIZE; idx++) {
            bus->memory[idx] = 0;
        }

        FILE *rom_file = fopen(rom_name, "r");
        if (rom_file == NULL) {
            *error = OPEN_ROM_FILE_FAILURE;
            free(bus);
            return bus;
        }

        if (fread(bus->memory, sizeof(uint8_t), ROM_SIZE, rom_file) != ROM_SIZE) {
            // TODO: ROM_FILE_TOO_LARGE
            *error = ROM_FILE_TOO_SMALL;
            free(bus);
            return bus;
        }

        if (fclose(rom_file) != 0) {

            *error =  CLOSE_ROM_FILE_FAILURE;
            free(bus);
            return bus;
        }

        return bus;
    }

    void release(Bus *const bus) {
        free(bus);
    }

    uint8_t read(Bus *const bus, const uint8_t addr) {
        return bus->memory[addr];
    }

    bool write(Bus *const bus, const uint8_t addr, const uint8_t data) {
        switch (addr) {
            case HALT_EMULATOR:
                printf("Emulator halted.\n");
                return false;
            case SERIAL_OUTPUT:
                printf("%c", (char)data);
                break;
            default:
                if (addr > (ROM_BASE + ROM_SIZE - 1)) {
                    bus->memory[addr] = data;
                }
        }

        return true;
    }

    void disassemble(Bus *const bus, FILE *const output_stream, const uint8_t start_address, const uint8_t end_address) {
        uint8_t instruction;
        uint8_t operand;
        bool consume_operand;

        for (uint8_t index = start_address; index < end_address; index++) {
            instruction = bus->memory[index];
            operand = bus->memory[index + 1];
            consume_operand = dis::disassemble_instruction(output_stream, index, instruction, operand);

            if (consume_operand) {
                index++;
            }
        }
    }
}
