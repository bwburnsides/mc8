#ifndef DISASM_H
#define DISASM_H

#include <stdint.h>
#include <stdio.h>

namespace dis {
    void decode_instruction(uint8_t instruction, uint8_t* opcode, uint8_t* immediate);
    bool disassemble_instruction(FILE* output_stream, const uint8_t address, uint8_t instruction, uint8_t operand);
}

#endif
