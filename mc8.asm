#once

#noemit on
#include "opcodes.py"
#noemit off

#bankdef base {
    #bits 8
    #addr 0x0000
    #size 0x0100
    #outp 0x0000 
}

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
    shl a                 => OP_SHIFT_RIGHT`8
    shr a                 => OP_SHIFT_RIGHT`8
    cmp a, b              => OP_COMPARE_A
    cmp a, {imm: i8}      => OP_COMPARE_A_IMMEDIATE`8 @ imm
    test a                => OP_TEST_A`8
    test [{addr: u8}]     => OP_TEST_MEMORY`8 @ addr
    and a, b              => OP_BITWISE_AND`8
    or a, b               => OP_BITWISE_OR`8
    xor a, b              => OP_BITWISE_XOR`8
    not a                 => OP_BITWISE_NOT_A`8
    load a, [{addr: u8}]  => OP_LOAD_A_MEMORY`8
    load a, [b]           => OP_LOAD_A_FROM_B`8
    load b, [{addr: u8}]  => OP_LOAD_B_MEMORY`8
    load b, [a]           => OP_LOAD_B_FROM_A`8
    store [{addr: u8}], a => OP_STORE_A_MEMORY`8
    store [b], a          => OP_STORE_A_AT_B`8
    store [{addr: u8}], b => OP_STORE_B_MEMORY`8
    store [a], b          => OP_STORE_B_AT_A`8
}
