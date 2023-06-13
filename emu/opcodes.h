#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

namespace opcode {
    const uint8_t NOP                         = 0b00000;
    const uint8_t SWAP                        = 0b00001;
    const uint8_t ZERO                        = 0b00010;
    const uint8_t SET_CARRY                   = 0b00011;
    const uint8_t CLEAR_CARRY                 = 0b00100;
    const uint8_t JUMP_CARRY                  = 0b00101;
    const uint8_t JUMP_NOT_CARRY              = 0b00110;
    const uint8_t JUMP_ZERO                   = 0b00111;
    const uint8_t JUMP_NOT_ZERO               = 0b01000;
    const uint8_t ADD_WITH_CARRY              = 0b01001;
    const uint8_t ADD_WITH_IMMEDIATE          = 0b01010;
    const uint8_t ADD_WITH_MEMORY             = 0b01011;
    const uint8_t ADD_WITH_SMALL_IMMEDIATE    = 0b01100;
    const uint8_t SUB_WITH_BORROW             = 0b01101;
    const uint8_t SHIFT_LEFT_SMALL_IMMEDIATE  = 0b01110;
    const uint8_t SHIFT_RIGHT_SMALL_IMMEDIATE = 0b01111;
    const uint8_t COMPARE_A_B                 = 0b10000;
    const uint8_t COMPARE_A_IMMEDIATE         = 0b10001;
    const uint8_t COMPARE_A_SMALL_IMMEDIATE   = 0b10010;
    const uint8_t TEST_MEMORY                 = 0b10011;
    const uint8_t BITWISE_AND                 = 0b10100;
    const uint8_t BITWISE_OR                  = 0b10101;
    const uint8_t BITWISE_XOR                 = 0b10110;
    const uint8_t BITWISE_NOT_A               = 0b10111;
    const uint8_t LOAD_A_MEMORY               = 0b11000;
    const uint8_t LOAD_A_FROM_B               = 0b11001;
    const uint8_t LOAD_B_MEMORY               = 0b11010;
    const uint8_t LOAD_B_FROM_A               = 0b11011;
    const uint8_t STORE_A_MEMORY              = 0b11100;
    const uint8_t STORE_A_AT_B                = 0b11101;
    const uint8_t STORE_B_MEMORY              = 0b11110;
    const uint8_t STORE_B_AT_A                = 0b11111;
}

#endif
