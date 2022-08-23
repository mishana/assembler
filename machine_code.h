//
// Created by misha on 17/08/2022.
//

#ifndef ASSEMBLER_MACHINE_CODE_H
#define ASSEMBLER_MACHINE_CODE_H

#include "parser.h"

typedef struct operand_t *Operand;
typedef struct machine_code_t *MachineCode;

MachineCode machineCodeCreate(Statement s, int ic);

int machineCodeCmp(MachineCode mc1, MachineCode mc2);

MachineCode machineCodeCopy(MachineCode mc);

void machineCodeDestroy(MachineCode mc);

size_t machineCodeGetSize(MachineCode mc);

#endif //ASSEMBLER_MACHINE_CODE_H
