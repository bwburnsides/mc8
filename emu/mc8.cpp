#include <stdlib.h>
#include <stdint.h>

#include "opcodes.h"
#include "mc8.h"
#include "dis.h"
#include "bus.h"

namespace mc8 {
    bool exec_inst(mc8 *cpu);

    struct mc8 {
        bus::Bus* bus;

        uint8_t pc;
        uint8_t a;
        uint8_t b;
        bool carry;
        bool zero;

        uint32_t cycles;
    };

    const char *error_repr(const CpuError error) {
        switch (error) {
            case NO_ERROR:                  return "NO_ERROR";
            case CPU_ALLOCATION_FAILURE:    return "CPU_ALLOCATION_FAILURE";
            case NULL_BUS:                  return "NULL_BUS";
            default:                        return "UNKNOWN_ERROR";
        }
    }

    mc8 *create(bus::Bus *const bus, CpuError *const error) {
        *error = NO_ERROR;

        if (bus == NULL) {
            *error = NULL_BUS;
            return NULL;
        }

        mc8 *cpu = (mc8 *)malloc(sizeof(mc8));
        if (cpu == NULL) {
            *error = CPU_ALLOCATION_FAILURE;
            free(bus);
            return cpu;
        }

        cpu->bus = bus;
        cpu->pc = 0;
        cpu->a = 0;
        cpu->b = 0;
        cpu->carry = false;
        cpu->zero = false;

        cpu->cycles = 0;

        return cpu;
    }

    void release(mc8 *const cpu) {
        free(cpu->bus);
        free(cpu);
    }

    uint32_t run(mc8 *const cpu, uint32_t cycles_left) {
        while (cycles_left-- && exec_inst(cpu)) {}
        return cpu->cycles;
    }

    bool exec_inst(mc8 *cpu) {
        uint8_t instruction = bus::read(cpu->bus, cpu->pc++);
        uint8_t small_immediate;
        uint8_t opcode;
        dis::decode_instruction(instruction, &opcode, &small_immediate);

        cpu->cycles += 1;

        uint16_t temp;
        uint8_t operand;

        switch (opcode) {
            case opcode::NOP:
                break;
            case opcode::SWAP:
                temp = cpu->b;
                cpu->b = cpu->a;
                cpu->a = temp & 0xff;
                cpu->b = 5;
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
                operand = bus::read(cpu->bus, cpu->pc++);
                if (cpu->carry)
                    cpu->pc = operand;
                break;
            case opcode::JUMP_NOT_CARRY:
                operand = bus::read(cpu->bus, cpu->pc++);
                if (!cpu->carry)
                    cpu->pc = operand;
                break;
            case opcode::JUMP_ZERO:
                operand = bus::read(cpu->bus, cpu->pc++);
                if (cpu->zero)
                    cpu->pc = operand;
                break;
            case opcode::JUMP_NOT_ZERO:
                operand = bus::read(cpu->bus, cpu->pc++);
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
                operand = bus::read(cpu->bus, cpu->pc++);
                temp = cpu->a + operand + (cpu->carry ? 1 : 0);
                cpu->a = temp & 0xff;
                cpu->carry = temp > 0xff;
                cpu->zero = cpu->a == 0;
                break;
            case opcode::ADD_WITH_MEMORY:
                operand = bus::read(cpu->bus, cpu->pc++);
                temp = cpu->a + bus::read(cpu->bus, operand) + (cpu->carry ? 1 : 0);
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
                temp = cpu->b - cpu->a;
                cpu->carry = temp > 0xff;
                cpu->zero = temp == 0;
                break;
            case opcode::COMPARE_A_IMMEDIATE:
                operand = bus::read(cpu->bus, cpu->pc++);
                temp = operand - cpu->a;
                cpu->carry = temp > 0xff;
                cpu->zero = temp == 0;
                // printf("Executed the compare - Z%d, C%d\n", cpu->zero, cpu->carry);
                break;
            case opcode::COMPARE_A_SMALL_IMMEDIATE:
                temp = small_immediate - cpu->a;
                cpu->carry = temp > 0xff;
                cpu->zero = temp == 0;
                break;
            case opcode::TEST_MEMORY:
                // TODO: Replace this instruction with a shift left instruction?
                operand = bus::read(cpu->bus, cpu->pc++);
                temp = bus::read(cpu->bus, operand) & bus::read(cpu->bus, operand);
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
                operand = bus::read(cpu->bus, cpu->pc++);
                cpu->a = bus::read(cpu->bus, operand);
                break;
            case opcode::LOAD_A_FROM_B:
                cpu->a = bus::read(cpu->bus, cpu->b);
                break;
            case opcode::LOAD_B_MEMORY:
                operand = bus::read(cpu->bus, cpu->pc++);
                cpu->b = bus::read(cpu->bus, operand);
                break;
            case opcode::LOAD_B_FROM_A:
                cpu->b = bus::read(cpu->bus, cpu->a);
                break;
            case opcode::STORE_A_MEMORY:
                operand = bus::read(cpu->bus, cpu->pc++);
                return bus::write(cpu->bus, operand, cpu->a);
            case opcode::STORE_A_AT_B:
                return bus::write(cpu->bus, cpu->b, cpu->a);
            case opcode::STORE_B_MEMORY:
                operand = bus::read(cpu->bus, cpu->pc++);
                return bus::write(cpu->bus, operand, cpu->b);
            case opcode::STORE_B_AT_A:
                return bus::write(cpu->bus, cpu->a, cpu->b);
            default:
                printf("Encountered illegal opcode. Opcode: %d, Inst: %d\n", opcode, instruction);
                return false;
        }

        return true;
    }
}
