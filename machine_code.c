//
// Created by misha on 17/08/2022.
//

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "machine_code.h"
#include "linkedlist.h"
#include "parser.h"
#include "errors.h"
#include "const_tables.h"
#include "str_utils.h"

#define MAX_OPERANDS_COUNT 2

struct machine_code_t {
    int line_num;
    int address; // 0-255

    int opcode; // 0-15
    int coding_method;  // A, E, R

    // first dest then src
    AddressingMode addressing_modes[MAX_OPERANDS_COUNT];

    int values[MAX_OPERANDS_COUNT]; // 0-255

    int registers[MAX_OPERANDS_COUNT]; // 0-15

    int label_addresses[MAX_OPERANDS_COUNT]; // 0-255

    int struct_addresses[MAX_OPERANDS_COUNT]; // 0-255
    int struct_field_nums[MAX_OPERANDS_COUNT]; // 1/2

    size_t size;
};


MachineCode machineCodeCreate(Statement s, int ic) {
    MachineCode mc = malloc(sizeof(*mc));
    if (!mc) {
        memoryAllocationError();
    }

    mc->line_num = statementGetLineNum(s);
    mc->address = ic;
    mc->opcode = getInstructionCode(statementGetMnemonic(s));
    mc->size = 1; // opcode word

    List instruction_operands = statementGetOperands(s);
    int num_operands = listLength(instruction_operands);
    assert(num_operands == getInstructionNumberOfOperands(statementGetMnemonic(s)));

    mc->addressing_modes[0] = EMPTY_ADDRESSING;
    mc->addressing_modes[1] = EMPTY_ADDRESSING;

    for (int i = 0; i < num_operands; ++i) {
        mc->size++; // operand value/address word

        /* It's iterating over the operands in reverse order. first dest then src. */
        const char *operand = listGetDataAt(instruction_operands, 1 - i);

        AddressingMode addressing_mode = getAddressingMode(operand);
        mc->addressing_modes[i] = addressing_mode;

        if (addressing_mode == IMMEDIATE_ADDRESSING) {
            mc->values[i] = atoi(operand + 1);
        } else if (addressing_mode == REGISTER_ADDRESSING) {
            mc->registers[i] = atoi(operand + 1);
        } else if (addressing_mode == STRUCT_ADDRESSING) {
            mc->size++; // struct field num word

            List split_operand = strSplit(operand, ".");
            const char *after_delim = listGetDataAt(split_operand, 1);

            mc->struct_field_nums[i] = atoi(after_delim);
        }
    }
    return mc;
}

int machineCodeCmp(MachineCode mc1, MachineCode mc2) {
    return mc1->address - mc2->address;
}

MachineCode machineCodeCopy(MachineCode mc) {
    MachineCode copy = malloc(sizeof(*copy));
    if (!copy) {
        memoryAllocationError();
    }
    memcpy(copy, mc, sizeof(*copy));
    return copy;
}

void machineCodeDestroy(MachineCode mc) {
    free(mc);
}

size_t machineCodeGetSize(MachineCode mc) {
    return mc->size;
}



