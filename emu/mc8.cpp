#include <stdint.h>
#include <stdlib.h>
#include "mc8.h"
#include "opcodes.h"

#include <stdio.h>

namespace mc8 {
    bool exec_inst(mc8* cpu);

    struct mc8 {
        uint8_t pc;
        uint8_t a;
        uint8_t b;
        bool carry;
        bool zero;

        BusRead read;
        BusWrite write;
        size_t cycles;
    };

    mc8* create(BusRead read, BusWrite write) {
        mc8* cpu = (mc8*) malloc(sizeof(mc8));
        cpu->pc = 0;
        cpu->read = read;
        cpu->write = write;
        cpu->cycles = 0;
        return cpu;
    }

    void release(mc8* cpu) {
        free(cpu);
    }

    size_t run(mc8* cpu) {
        while (exec_inst(cpu)) {}
        printf("a=%d, b=%d, pc=%d\n", cpu->a, cpu->b, cpu->pc);
        return cpu->cycles;
    }

    bool exec_inst(mc8* cpu) {
        uint8_t instruction = cpu->read(cpu->pc++);
        uint8_t small_immediate = instruction & 0b111;
        uint8_t opcode = (instruction >> 3) & 0b11111;
        cpu->cycles += 4;

        uint16_t temp;
        uint8_t operand;

        switch (opcode) {
            case opcode::NOP:
                break;
            case opcode::SWAP:
                temp = cpu->b;
                cpu->b = cpu->a;
                cpu->a = temp & 0xff;
                break;
            case opcode::ZERO:
                cpu->a = 0;
                cpu->b = 0;
                break;
            case opcode::SET_CARRY:
                cpu->carry = true;
                break;
            case opcode::CLEAR_CARRY:
                cpu->carry = false;
                break;
            case opcode::JUMP_CARRY:
                operand = cpu->read(cpu->pc++);
                if (cpu->carry)
                    cpu->pc = operand;
                break;
            case opcode::JUMP_NOT_CARRY:
                operand = cpu->read(cpu->pc++);
                if (!cpu->carry)
                    cpu->pc = operand;
                break;
            case opcode::JUMP_ZERO:
                operand = cpu->read(cpu->pc++);
                if (cpu->zero)
                    cpu->pc = operand;
                break;
            case opcode::JUMP_NOT_ZERO:
                operand = cpu->read(cpu->pc++);
                if (!cpu->zero)
                    cpu->pc = operand;
                break;
            case opcode::ADD_WITH_CARRY:
                temp = cpu->a + cpu->b + (cpu->carry ? 1 : 0);
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::ADD_WITH_IMMEDIATE:
                operand = cpu->read(cpu->pc++);
                temp = cpu->a + operand + (cpu->carry ? 1 : 0);
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::ADD_WITH_MEMORY:
                operand = cpu->read(cpu->pc++);
                temp = cpu->a + cpu->read(operand) + (cpu->carry ? 1 : 0);
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::ADD_WITH_SMALL_IMMEDIATE:
                temp = cpu->a + small_immediate + (cpu->carry ? 1 : 0);
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::SUB_WITH_BORROW:
                // TODO: How does subtraction even work? lol
                temp = cpu->a - cpu->b - (cpu->carry ? 1 : 0);
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::SHIFT_LEFT_SMALL_IMMEDIATE:
                temp = cpu->a << small_immediate;
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::SHIFT_RIGHT_SMALL_IMMEDIATE:
                temp = cpu->a >> small_immediate;
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::COMPARE_A_B:
                temp = cpu->a - cpu->b;
                cpu->carry = temp > 0xff;
                cpu->zero = temp == 0;
                break;
            case opcode::COMPARE_A_IMMEDIATE:
                operand = cpu->read(cpu->pc++);
                temp = cpu->a - operand;
                cpu->carry = temp > 0xff;
                cpu->zero = temp == 0;
                break;
            case opcode::COMPARE_A_SMALL_IMMEDIATE:
                temp = cpu->a - small_immediate;
                cpu->carry = temp > 0xff;
                cpu->zero = temp == 0;
                break;
            case opcode::TEST_MEMORY:
                // TODO: Replace this instruction with a shift left instruction?
                operand = cpu->read(cpu->pc++);
                temp = cpu->read(operand) & cpu->read(operand);
                cpu->carry = false;
                cpu->zero = temp == 0;
                break;
            case opcode::BITWISE_AND:
                cpu->a = cpu->a & cpu->b;
                cpu->carry = false;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::BITWISE_OR:
                cpu->a = cpu->a | cpu->b;
                cpu->carry = false;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::BITWISE_XOR:
                cpu->a = cpu->a ^ cpu->b;
                cpu->carry = false;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::BITWISE_NOT_A:
                cpu->a = ~cpu->a;
                cpu->carry = false;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::LOAD_A_MEMORY:
                operand = cpu->read(cpu->pc++);
                cpu->a = cpu->read(operand);
                break;
            case opcode::LOAD_A_FROM_B:
                cpu->a = cpu->read(cpu->b);
                break;
            case opcode::LOAD_B_MEMORY:
                operand = cpu->read(cpu->pc++);
                cpu->b = cpu->read(operand);
                break;
            case opcode::LOAD_B_FROM_A:
                cpu->b = cpu->read(cpu->a);
                break;
            case opcode::STORE_A_MEMORY:
                operand = cpu->read(cpu->pc++);
                return cpu->write(operand, cpu->a);
            case opcode::STORE_A_AT_B:
                return cpu->write(cpu->b, cpu->a);
            case opcode::STORE_B_MEMORY:
                operand = cpu->read(cpu->pc++);
                return cpu->write(operand, cpu->b);
            case opcode::STORE_B_AT_A:
                return cpu->write(cpu->a, cpu->b);
            default:
                return false;
        }

        return true;
    }

};
