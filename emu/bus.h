#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <stdlib.h>

namespace bus {
    extern const uint8_t ROM_BASE;
    extern const uint16_t ROM_SIZE;

    enum BusError {
        NO_ERROR,
        OPEN_ROM_FILE_FAILURE,
        ROM_FILE_TOO_SMALL,
        CLOSE_ROM_FILE_FAILURE,
    };

    const char* error_repr(const BusError error);

    BusError init(const char *const rom_name);

    uint8_t read(uint8_t addr);
    bool write(uint8_t addr, uint8_t data);

    void disassemble(FILE* const output_stream, const uint8_t start_address, const uint8_t end_address);
}

#endif
