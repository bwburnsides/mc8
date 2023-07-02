#ifndef BUS_H
#define BUS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// TODO: Generalize address and data widths for emulation system.
// TODO: Provide control line features to bus interactions for emulation system.
/// @brief Contains declarations relating to 8-bit bus interactions.
namespace bus {

    /// @brief An 8-bit bus with associated state.
    typedef struct Bus Bus;

    /// @brief Starting address of ROM region on bus.
    extern const uint8_t ROM_BASE;

    /// @brief Size of ROM region on bus.
    extern const uint16_t ROM_SIZE;

    /// @brief Error cases over life cycle of bus operation
    enum BusError {
        NO_ERROR,
        BUS_ALLOCATION_FAILURE,
        OPEN_ROM_FILE_FAILURE,
        ROM_FILE_TOO_SMALL,
        CLOSE_ROM_FILE_FAILURE,
    };

    /// @brief Return a C-string representation of the provided BusError.
    /// @param[in] error BusError to represent.
    /// @return C-string representation of error.
    const char* error_repr(const BusError error);

    /// @brief Create and initialize a bus via ROM file with provided path.
    /// @param[in] rom_name Path to binary for bus representation. Format is implementation specific.
    /// @param[out] error Output representing error state of initialization.
    /// @return Initialized bus.
    /// @note Bus is dynamically allocated and owned by the caller.
    Bus *create(char *const rom_name, BusError *const error);

    /// @brief Destruct bus.
    /// @param[in] bus Bus to destroy.
    /// @note Bus is unusable following call to release.
    void release(Bus *const bus);

    // TODO: read should return bool to designate success like write.
    /// @brief Read a value from bus.
    /// @param[in] bus Bus to read from.
    /// @param[in] addr Address to read from on bus.
    /// @return Value read from bus.
    uint8_t read(Bus *const bus, const uint8_t addr);

    // TODO: write should return BusError to designate success.
    /// @brief Write a value to bus.
    /// @param[in] bus Bus to write to.
    /// @param[in] addr Address to write to on bus.
    /// @param[in] data Value to write to bus.
    /// @return Whether write was successful. Should be considered a bus error.
    bool write(Bus *const bus, const uint8_t addr, const uint8_t data);

    /// @brief Disassemble data within address range.
    /// @param[in] bus Bus to disassemble via.
    /// @param[in] output_stream Stream to write disassembled data to.
    /// @param[in] start_address Starting address to begin disassembly from.
    /// @param[in] end_address End address to stop disassembly at.
    void disassemble(Bus *const bus, FILE *const output_stream, const uint8_t start_address, const uint8_t end_address);
}

#endif
