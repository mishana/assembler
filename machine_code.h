//
// Created by misha on 17/08/2022.
//

#ifndef ASSEMBLER_MACHINE_CODE_H
#define ASSEMBLER_MACHINE_CODE_H

#include "parser.h"


typedef struct machine_code_t *MachineCode;

MachineCode machineCodeCreate(Statement s, int ic);

int machineCodeCmp(MachineCode mc1, MachineCode mc2);

MachineCode machineCodeCopy(MachineCode mc);

void machineCodeDestroy(MachineCode mc);

size_t machineCodeGetSize(MachineCode mc);

int machineCodeGetNumOperands(MachineCode mc);

const char *machineCodeGetOperand(MachineCode mc, int index);

bool machineCodeGetIsExternOperand(MachineCode mc, int index);

const char *machineCodeGetExternalOperandBase32Address(MachineCode mc, int index);

bool machineCodeUpdateFromSymtab(MachineCode mc, List symtab, const char *filename_suffix, const char *filename,
                                 int start_address_offset);

void machineCodeToObjFile(MachineCode mc, FILE *f, int start_address_offset);

#endif //ASSEMBLER_MACHINE_CODE_H
