#once

#noemit on
#include "opcodes.py"
#noemit off

#ruledef mc8 {
    nop                   => OP_NOP`8
    swap                  => OP_SWAP`8
    zero                  => OP_ZERO`8
    set                   => OP_SET_CARRY`8
    clear                 => OP_CLEAR_CARRY`8
    jmp.c {addr: u8}      => OP_JUMP_CARRY`8 @ addr
    jmp.nc {addr: u8}     => OP_JUMP_NOT_CARRY`8 @ addr
    jmp.z {addr: u8}      => OP_JUMP_ZERO`8 @ addr
    jmp.nz {addr: u8}     => OP_JUMP_NOT_ZERO`8 @ addr
    addc a, b             => OP_ADD_WITH_CARRY`8
    addc a, {imm: i8}     => OP_ADD_WITH_IMMEDIATE`8 @ imm
    addc a, [{addr: u8}]  => OP_ADD_WITH_MEMORY`8 @ addr
    addc a, {imm: i3}     => OP_ADD_WITH_SMALL_IMMEDIATE`5 @ imm
    subb a, b             => OP_SUB_WITH_BORROW`8
    shl a, {imm: u3}      => OP_SHIFT_RIGHT_SMALL_IMMEDIATE`5 @ imm
    shr a, {imm: u3}      => OP_SHIFT_RIGHT_SMALL_IMMEDIATE`5 @ imm
    cmp a, b              => OP_COMPARE_A
    cmp a, {imm: i8}      => OP_COMPARE_A_IMMEDIATE`8 @ imm
    cmp a, {imm: i3}      => OP_COMPARE_A_SMALL_IMMEDIATE`5 @ imm
    test [{addr: u8}]     => OP_TEST_MEMORY`8 @ addr
    and a, b              => OP_BITWISE_AND`8
    or a, b               => OP_BITWISE_OR`8
    xor a, b              => OP_BITWISE_XOR`8
    not a                 => OP_BITWISE_NOT_A`8
    load a, [{addr: u8}]  => OP_LOAD_A_MEMORY`8 @ addr
    load a, [b]           => OP_LOAD_A_FROM_B`8
    load b, [{addr: u8}]  => OP_LOAD_B_MEMORY`8 @ addr
    load b, [a]           => OP_LOAD_B_FROM_A`8
    store [{addr: u8}], a => OP_STORE_A_MEMORY`8 @ addr
    store [b], a          => OP_STORE_A_AT_B`8
    store [{addr: u8}], b => OP_STORE_B_MEMORY`8 @ addr
    store [a], b          => OP_STORE_B_AT_A`8
}
