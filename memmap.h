//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_MEMMAP_H
#define ASSEMBLER_MEMMAP_H

#include <stdio.h>
#include "parser.h"

typedef struct memory_code_t *MemoryCode;

MemoryCode memoryCodeCreate(Statement s, int dc);

int memoryCodeCmp(MemoryCode mc1, MemoryCode mc2);

MemoryCode memoryCodeCopy(MemoryCode mc);

void memoryCodeDestroy(MemoryCode mc);

size_t memoryCodeGetSize(MemoryCode mc);

void memoryCodeSetStartAddress(MemoryCode mc, int address);

int memoryCodeGetStartAddress(MemoryCode mc);

size_t calcDirectiveDataSize(Statement s);

void memoryCodeToObjFile(MemoryCode mc, FILE *f);

#endif //ASSEMBLER_MEMMAP_H
