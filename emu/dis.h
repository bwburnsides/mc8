#ifndef DISASM_H
#define DISASM_H

#include <stdio.h>
#include <stdint.h>

/// @brief Declarations related to decoding and disassembly of data.
namespace dis {

    // TODO: Generalize parameter types for emulation system.
    /// @brief Decode instruction into constituent elements.
    /// @param[in] instruction Encoded instruction to decode.
    /// @param[out] opcode Decoded opcode from instruction.
    /// @param[out] immediate Decoded immediate from instruction.
    void decode_instruction(const uint8_t instruction, uint8_t *const opcode, uint8_t *const immediate);

    // TODO: Generalize parameter and return types for emulation system.
    /// @brief Disassemble instruction found at specified address.
    /// @param[in] output_stream Stream to write disassembly to.
    /// @param[in] address Address instruction is found at.
    /// @param[in] instruction Instruction to disassemble.
    /// @param[in] operand Optional operand for given instruction.
    /// @return Whether the operand was used in instruction disassembly.
    bool disassemble_instruction(FILE *const output_stream, const uint8_t address, const uint8_t instruction, const uint8_t operand);
}

#endif
