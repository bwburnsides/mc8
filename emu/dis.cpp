#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "opcodes.h"
#include "dis.h"

namespace dis {
    void decode_instruction(const uint8_t instruction, uint8_t *const opcode, uint8_t *const immediate) {
        *immediate = (instruction >> 5) & 0b111;
        *opcode = instruction & 0b11111;
    }

    bool disassemble_instruction(FILE *const output_stream, const uint8_t address, const uint8_t instruction, const uint8_t operand) {
        bool consume_operand = false;

        uint8_t opcode;
        uint8_t immediate;
        decode_instruction(instruction, &opcode, &immediate);

        fprintf(output_stream, "0x%02x: ", address);

        switch (opcode) {
            case opcode::NOP:
                fprintf(output_stream, "%s", "nop");
                break;
            case opcode::SWAP:
                fprintf(output_stream, "%s", "swap");
                break;
            case opcode::ZERO:
                fprintf(output_stream, "%s", "zero");
                break;
            case opcode::SET_CARRY:
                fprintf(output_stream, "%s", "set");
                break;
            case opcode::CLEAR_CARRY:
                fprintf(output_stream, "%s", "clear");
                break;
            case opcode::JUMP_CARRY:
                fprintf(output_stream, "%s 0x%02x", "jmp.c", operand);
                consume_operand = true;
                break;
            case opcode::JUMP_NOT_CARRY:
                fprintf(output_stream, "%s 0x%02x", "jmp.nc", operand);
                consume_operand = true;
                break;
            case opcode::JUMP_ZERO:
                fprintf(output_stream, "%s 0x%02x", "jmp.z", operand);
                consume_operand = true;
                break;
            case opcode::JUMP_NOT_ZERO:
                fprintf(output_stream, "%s 0x%02x", "jmp.nz", operand);
                consume_operand = true;
                break;
            case opcode::ADD_WITH_CARRY:
                fprintf(output_stream, "%s a, b", "addc");
                break;
            case opcode::ADD_WITH_IMMEDIATE:
                fprintf(output_stream, "%s a, 0x%02x", "addc", operand);
                consume_operand = true;
                break;
            case opcode::ADD_WITH_MEMORY:
                fprintf(output_stream, "%s a, [0x%02x]", "addc", operand);
                consume_operand = true;
                break;
            case opcode::ADD_WITH_SMALL_IMMEDIATE:
                fprintf(output_stream, "%s a, 0x%02x", "addc", immediate);
                break;
            case opcode::SUB_WITH_BORROW:
                fprintf(output_stream, "%s", "subb a, b");
                break;
            case opcode::SHIFT_LEFT_SMALL_IMMEDIATE:
                fprintf(output_stream, "%s a, 0x%02x", "shl", immediate);
                break;
            case opcode::SHIFT_RIGHT_SMALL_IMMEDIATE:
                fprintf(output_stream, "%s a, 0x%02x", "shr", immediate);
                break;
            case opcode::COMPARE_A_B:
                fprintf(output_stream, "%s a, b", "cmp");
                break;
            case opcode::COMPARE_A_IMMEDIATE:
                fprintf(output_stream, "%s a, 0x%02x", "cmp", operand);
                consume_operand = true;
                break;
            case opcode::COMPARE_A_SMALL_IMMEDIATE:
                fprintf(output_stream, "%s a, 0x%02x", "cmp", immediate);
                break;
            case opcode::TEST_MEMORY:
                fprintf(output_stream, "%s [0x%02x]", "test", operand);
                consume_operand = true;
                break;
            case opcode::BITWISE_AND:
                fprintf(output_stream, "%s a", "and");
                break;
            case opcode::BITWISE_OR:
                fprintf(output_stream, "%s a", "or");
                break;
            case opcode::BITWISE_XOR:
                fprintf(output_stream, "%s a", "xor");
                break;
            case opcode::BITWISE_NOT_A:
                fprintf(output_stream, "%s a", "not");
                break;
            case opcode::LOAD_A_MEMORY:
                fprintf(output_stream, "%s a, [0x%02x]", "load", operand);
                consume_operand = true;
                break;
            case opcode::LOAD_A_FROM_B:
                fprintf(output_stream, "%s a, [b]", "load");
                break;
            case opcode::LOAD_B_MEMORY:
                fprintf(output_stream, "%s b, [0x%02x]", "load", operand);
                consume_operand = true;
                break;
            case opcode::LOAD_B_FROM_A:
                fprintf(output_stream, "%s b, [a]", "load");
                break;
            case opcode::STORE_A_MEMORY:
                fprintf(output_stream, "%s [0x%02x], a", "store", operand);
                consume_operand = true;
                break;
            case opcode::STORE_A_AT_B:
                fprintf(output_stream, "%s [b], a", "store");
                break;
            case opcode::STORE_B_MEMORY:
                fprintf(output_stream, "%s [0x%02x], b", "store", operand);
                consume_operand = true;
                break;
            case opcode::STORE_B_AT_A:
                fprintf(output_stream, "%s [a], b", "store");
                break;
            default:
                break;
        }

        fprintf(output_stream, "\n");
        return consume_operand;
    }

}
