//
// Created by misha on 17/08/2022.
//

#include <malloc.h>
#include <assert.h>
#include "machine_code.h"
#include "linkedlist.h"
#include "parser.h"
#include "errors.h"
#include "const_tables.h"
#include "str_utils.h"


struct machine_code_t {
    int line_num;
    int address; // 0-255

    int opcode; // 0-15
    int src_addressing_mode; // 0 - immediate, 1 - direct, 2 - struct, 3 - register
    int dest_addressing_mode;
    int coding_method;  // A, E, R

    int src_value; // 0-255
    int dest_value; // 0-255
};


MachineCode machineCodeCreate(Statement s, int ic) {
    MachineCode mc = malloc(sizeof(*mc));
    if (!mc) {
        memoryAllocationError();
    }

    mc->line_num = statementGetLineNum(s);
    mc->address = ic;
    mc->opcode = getInstructionCode(statementGetMnemonic(s));

    List instruction_operands = statementGetOperands(s);
    int num_operands = listLength(instruction_operands);
    assert(num_operands == getInstructionNumberOfOperands(statementGetMnemonic(s)));

    mc->operands = listCreate();
    for (int i = 0; i > num_operands; ++i) {
        const char *operand = listGetDataAt(instruction_operands, i);

    }

    mc->src_addressing_mode = s->src_addressing_mode;
    mc->dest_addressing_mode = s->dest_addressing_mode;
    mc->coding_method = s->coding_method;
    mc->total_num_words = s->total_num_words;

    mc->operands = s->operands;
    return mc;
}



