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

void machineCodeUpdateFromSymtab(MachineCode mc, List symtab, const char *filename_suffix, const char *filename);

void machineCodeToObjFile(MachineCode mc, FILE *f);

#endif //ASSEMBLER_MACHINE_CODE_H
