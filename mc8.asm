#once

OP_NOP                        = 0b00000  ; 0x00
OP_SWAP                       = 0b00001  ; 0x01
OP_SET_CARRY                  = 0b00010  ; 0x02
OP_CLEAR_CARRY                = 0b00011  ; 0x03
OP_JUMP_CARRY                 = 0b00100  ; 0x04
OP_JUMP_NOT_CARRY             = 0b00101  ; 0x05
OP_JUMP_ZERO                  = 0b00110  ; 0x06
OP_JUMP_NOT_ZERO              = 0b00111  ; 0x07
OP_ADD_WITH_CARRY             = 0b01000  ; 0x08
OP_ADD_WITH_IMMEDIATE         = 0b01001  ; 0x09
OP_ADD_WITH_SMALL_IMMEDIATE   = 0b01010  ; 0x0A
OP_SUB_WITH_BORROW            = 0b01011  ; 0x0B
OP_SHIFT_LEFT                 = 0b01100  ; 0x0C
OP_SHIFT_RIGHT                = 0b01101  ; 0x0D
OP_ARITHMETIC_SHIFT_RIGHT     = 0b01110  ; 0x0E
OP_COMPARE_A_B                = 0b01111  ; 0x0F
OP_COMPARE_A_IMMEDIATE        = 0b10000  ; 0x10
OP_TEST_A                     = 0b10001  ; 0x11
OP_TEST_MEMORY                = 0b10010  ; 0x12
OP_BITWISE_AND                = 0b10011  ; 0x13
OP_BITWISE_OR                 = 0b10100  ; 0x14
OP_BITWISE_XOR                = 0b10101  ; 0x15
OP_BITWISE_NOT_A              = 0b10110  ; 0x16
OP_BITWISE_NOT_B              = 0b10111  ; 0x17
OP_LOAD_A_MEMORY              = 0b11000  ; 0x18
OP_LOAD_A_FROM_B              = 0b11001  ; 0x19
OP_LOAD_B_MEMORY              = 0b11010  ; 0x1A
OP_LOAD_B_FROM_A              = 0b11011  ; 0x1B
OP_STORE_A_MEMORY             = 0b11100  ; 0x1C
OP_STORE_A_AT_B               = 0b11101  ; 0x1D
OP_STORE_B_MEMORY             = 0b11110  ; 0x1E
OP_STORE_B_AT_A               = 0b11111  ; 0x1F

#bankdef base {
    #bits 8
    #addr 0x0000
    #size 0x0100
    #outp 0x0000 
}

#ruledef mc8 {
    nop                   => OP_NOP`8
    swap                  => OP_SWAP`8
    set                   => OP_SET_CARRY`8
    clear                 => OP_CLEAR_CARRY`8
    jmp.c {addr: u8}      => OP_JUMP_CARRY`8 @ addr
    jmp.nc {addr: u8}     => OP_JUMP_NOT_CARRY`8 @ addr
    jmp.z {addr: u8}      => OP_JUMP_ZERO`8 @ addr
    jmp.nz {addr: u8}     => OP_JUMP_NOT_ZERO`8 @ addr
    addc a, b             => OP_ADD_WITH_CARRY`8
    addc a, {imm: i8}     => OP_ADD_WITH_IMMEDIATE`8 @ imm
    addc a, {imm: i3}     => OP_ADD_WITH_SMALL_IMMEDIATE`5 @ imm
    subb a, b             => OP_SUB_WITH_BORROW`8
    shl a                 => OP_SHIFT_RIGHT`8
    shr a                 => OP_SHIFT_RIGHT`8
    asr a                 => OP_ARITHMETIC_SHIFT_RIGHT`8
    cmp a, b              => OP_COMPARE_A
    cmp a, {imm: i8}      => OP_COMPARE_A_IMMEDIATE`8 @ imm
    test a                => OP_TEST_A`8
    test [{addr: u8}]     => OP_TEST_MEMORY`8 @ addr
    and a, b              => OP_BITWISE_AND`8
    or a, b               => OP_BITWISE_OR`8
    xor a, b              => OP_BITWISE_XOR`8
    not a                 => OP_BITWISE_NOT_A`8
    not b                 => OP_BITWISE_NOT_B`8
    load a, [{addr: u8}]  => OP_LOAD_A_MEMORY`8
    load a, [b]           => OP_LOAD_A_FROM_B`8
    load b, [{addr: u8}]  => OP_LOAD_B_MEMORY`8
    load b, [a]           => OP_LOAD_B_FROM_A`8
    store [{addr: u8}], a => OP_STORE_A_MEMORY`8
    store [b], a          => OP_STORE_A_AT_B`8
    store [{addr: u8}], b => OP_STORE_B_MEMORY`8
    store [a], b          => OP_STORE_B_AT_A`8
}
