import enum
from opcodes import *


class Control(enum.IntEnum):
    # Line 1
    LoadProgramCounter = 1 << 0

    # Line 2
    AddressSelect = 1 << 1

    # Line 3, 4
    RightSelectB = 0 << 2
    RightSelectTemp = 1 << 2
    RightSelectIR = 2 << 2
    RightSelectA = 3 << 2

    # Line 5, 6, 7
    LoadTemp = 0 << 4
    LoadMemoryAddressRegister = 1 << 4
    LoadA = 2 << 4
    LoadB = 3 << 4
    LoadIR = 4 << 4
    JumpEnable = 5 << 4
    LoadMemory = 6 << 4

    # Line 8, 9, 10
    AssertAlu = 0 << 7
    AssertA = 1 << 7
    AssertB = 2 << 7
    AssertMemory = 3 << 7
    AssertTemp = 4 << 7

    # Line 11, 12, 13, 14
    AluNOP = 0 << 10
    AluADD = 1 << 10
    AluSUB = 2 << 10
    AluSHL = 3 << 10
    AluSHR = 4 << 10
    AluAND = 5 << 10
    AluOR = 6 << 10
    AluXOR = 7 << 10
    AluNOT = 8 << 10
    AluSET = 9 << 10
    AluCLEAR = 10 << 10


def decode_control_lines(opcode: int, carry_flag: bool, zero_flag: bool) -> list[int]:
    operations = [FETCH, 0, 0, 0]

    if opcode == OP_NOP:
        ...

    if opcode == OP_ZERO:
        operations[1] = Control.RightSelectA | Control.AluXOR | Control.AssertAlu | Control.LoadA
        operations[2] = Control.AssertA | Control.LoadB

    if opcode == OP_SWAP:
        operations[1] = Control.AssertA | Control.LoadTemp
        operations[2] = Control.AssertB | Control.LoadA
        operations[3] = Control.AssertTemp | Control.LoadB

    if opcode == OP_SET_CARRY:
        operations[1] = Control.AluSET

    if opcode == OP_CLEAR_CARRY:
        operations[1] = Control.AluCLEAR

    if opcode == OP_JUMP_CARRY:
        operations[1] = (
            Control.LoadProgramCounter | Control.AssertMemory | Control.LoadTemp
        )
        if carry_flag:
            operations[2] = (
                Control.AssertTemp | Control.LoadProgramCounter | Control.JumpEnable
            )

    if opcode == OP_JUMP_NOT_CARRY:
        operations[1] = (
            Control.LoadProgramCounter | Control.AssertMemory | Control.LoadTemp
        )
        if not carry_flag:
            operations[2] = (
                Control.AssertTemp | Control.LoadProgramCounter | Control.JumpEnable
            )

    if opcode == OP_JUMP_ZERO:
        operations[1] = (
            Control.LoadProgramCounter | Control.AssertMemory | Control.LoadTemp
        )
        if zero_flag:
            operations[2] = (
                Control.AssertTemp | Control.LoadProgramCounter | Control.JumpEnable
            )

    if opcode == OP_JUMP_NOT_ZERO:
        operations[1] = (
            Control.LoadProgramCounter | Control.AssertMemory | Control.LoadTemp
        )
        if not zero_flag:
            operations[2] = (
                Control.AssertTemp | Control.LoadProgramCounter | Control.JumpEnable
            )

    if opcode == OP_ADD_WITH_CARRY:
        operations[1] = Control.AluADD | Control.AssertAlu | Control.LoadA

    if opcode == OP_ADD_WITH_IMMEDIATE:
        operations[1] = (
            Control.LoadProgramCounter | Control.AssertMemory | Control.LoadTemp
        )
        operations[2] = (
            Control.AluADD | Control.RightSelectTemp | Control.AssertAlu | Control.LoadA
        )

    if opcode == OP_ADD_WITH_MEMORY:
        operations[1] = Control.LoadProgramCounter | Control.AssertMemory | Control.LoadMemoryAddressRegister
        operations[2] = Control.AddressSelect | Control.AssertMemory | Control.LoadTemp
        operations[3] = Control.AluADD | Control.AssertAlu | Control.LoadA

    if opcode == OP_ADD_WITH_SMALL_IMMEDIATE:
        operations[1] = (
            Control.AluADD | Control.RightSelectIR | Control.AssertAlu | Control.LoadA
        )

    if opcode == OP_SUB_WITH_BORROW:
        operations[1] = Control.AluSUB | Control.AssertAlu | Control.LoadA

    if opcode == OP_SHIFT_LEFT:
        operations[1] = Control.AluSHL | Control.AssertAlu | Control.LoadA

    if opcode == OP_SHIFT_RIGHT:
        operations[1] = Control.AluSHR | Control.AssertAlu | Control.LoadA

    if opcode == OP_COMPARE_A_B:
        operations[1] = Control.AluSET
        operations[2] = Control.AluSUB

    if opcode == OP_COMPARE_A_IMMEDIATE:
        operations[1] = (
            Control.LoadProgramCounter
            | Control.AssertMemory
            | Control.LoadTemp
            | Control.AluSET
        )
        operations[2] = Control.AluSUB

    if opcode == OP_TEST_A:
        operations[1] = Control.AluAND | Control.RightSelectA

    if opcode == OP_TEST_MEMORY:
        operations[1] = (
            Control.LoadProgramCounter | Control.AssertMemory | Control.LoadMemoryAddressRegister
        )
        operations[2] = Control.AddressSelect | Control.AssertMemory | Control.LoadTemp
        operations[3] = Control.AluAND | Control.RightSelectA

    if opcode == OP_BITWISE_AND:
        operations[1] = Control.AluAND | Control.AssertAlu | Control.LoadA

    if opcode == OP_BITWISE_OR:
        operations[1] = Control.AluOR | Control.AssertAlu | Control.LoadA

    if opcode == OP_BITWISE_XOR:
        operations[1] = Control.AluXOR | Control.AssertAlu | Control.LoadA

    if opcode == OP_BITWISE_NOT_A:
        operations[1] = Control.AluNOT | Control.AssertAlu | Control.LoadA

    if opcode == OP_LOAD_A_MEMORY:
        operations[1] = (
            Control.LoadProgramCounter
            | Control.AssertMemory
            | Control.LoadMemoryAddressRegister
        )
        operations[2] = Control.AddressSelect | Control.AssertMemory | Control.LoadA

    if opcode == OP_LOAD_A_FROM_B:
        operations[1] = Control.AssertB | Control.LoadMemoryAddressRegister
        operations[2] = Control.AddressSelect | Control.AssertMemory | Control.LoadA

    if opcode == OP_LOAD_B_MEMORY:
        operations[1] = (
            Control.LoadProgramCounter
            | Control.AssertMemory
            | Control.LoadMemoryAddressRegister
        )
        operations[2] = Control.AddressSelect | Control.AssertMemory | Control.LoadB

    if opcode == OP_LOAD_B_FROM_A:
        operations[1] = Control.AssertB | Control.LoadMemoryAddressRegister
        operations[2] = Control.AddressSelect | Control.AssertMemory | Control.LoadB

    if opcode == OP_STORE_A_MEMORY:
        operations[1] = (
            Control.LoadProgramCounter
            | Control.AssertMemory
            | Control.LoadMemoryAddressRegister
        )
        operations[1] = Control.AddressSelect | Control.LoadMemory | Control.AssertA

    if opcode == OP_STORE_A_AT_B:
        operations[1] = Control.AssertB | Control.LoadMemoryAddressRegister
        operations[2] = Control.AddressSelect | Control.LoadMemory | Control.AssertA

    if opcode == OP_STORE_B_MEMORY:
        operations[1] = (
            Control.LoadProgramCounter
            | Control.AssertMemory
            | Control.LoadMemoryAddressRegister
        )
        operations[2] = Control.AddressSelect | Control.LoadMemory | Control.AssertB

    if opcode == OP_STORE_B_AT_A:
        operations[1] = Control.AssertA | Control.LoadMemoryAddressRegister
        operations[2] = Control.AddressSelect | Control.LoadMemory | Control.AssertB

    return operations


def write_microcode(microcode: list[int], file) -> None:
    for operation in microcode:
        lines = operation.to_bytes(2, "little", signed=False)
        file.write(lines)


microcode = []
FETCH = Control.LoadProgramCounter | Control.AssertMemory | Control.LoadIR

for state in range(2**7):
    opcode = (state) & 0b11111
    carry_flag = bool((state >> 5) & 0b1)
    zero_flag = bool((state >> 6) & 0b1)

    micro_operations = decode_control_lines(opcode, carry_flag, zero_flag)

    microcode.extend(micro_operations)

with open("microcode.bin", "wb") as out:
    write_microcode(microcode, out)
