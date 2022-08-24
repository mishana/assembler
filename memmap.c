//
// Created by misha on 27/07/2022.
//

#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "memmap.h"
#include "const_tables.h"
#include "errors.h"


struct memory_code_t {
    int* values;
    int size;

    int start_address; // 0-255
};

MemoryCode memoryCodeCreate(Statement s, int dc) {
    MemoryCode mem_c = malloc(sizeof(*mem_c));
    if (!mem_c) {
        memoryAllocationError();
    }

    mem_c->start_address = dc;
    mem_c->size = calcDirectiveDataSize(s);
    mem_c->values = malloc(mem_c->size * sizeof(int));
    if (!mem_c->values) {
        memoryAllocationError();
    }

    const char *directive = statementGetMnemonic(s);
    List operands = statementGetOperands(s);
    int num_operands = listLength(operands);

    if (strcmp(directive, DIRECTIVE_DATA) == 0) {
        for (int i = 0; i < num_operands; i++) {
            mem_c->values[i] = atoi(listGetDataAt(operands, i));
        }
    } else if (strcmp(directive, DIRECTIVE_STRING) == 0) {
        const char *str = listGetDataAt(operands, 0);
        for (int i = 0; i < strlen(str + 1) - 1; i++) {
            mem_c->values[i] = str[1 + i]; // +1 to skip the first '"'
        }
        mem_c->values[mem_c->size - 1] = '\0';
    } else if (strcmp(directive, DIRECTIVE_STRUCT) == 0) {
        mem_c->values[0] = atoi(listGetDataAt(operands, 0));

        const char *str = listGetDataAt(statementGetOperands(s), 1);
        for (int i = 1; i <= strlen(str + 1) - 1; i++) {
            mem_c->values[i] = str[i]; // +1 to skip the first '"' is not needed since i starts from 1
        }
        mem_c->values[mem_c->size - 1] = '\0';
    } else { // .entry or .extern
        free(mem_c);
        return NULL;
    }

    return mem_c;
}

int memoryCodeCmp(MemoryCode mc1, MemoryCode mc2) {
    return mc1->start_address - mc2->start_address;
}

MemoryCode memoryCodeCopy(MemoryCode mc) {
    MemoryCode copy = malloc(sizeof(*copy));
    if (!copy) {
        memoryAllocationError();
    }

    copy->start_address = mc->start_address;
    copy->size = mc->size;
    copy->values = malloc(copy->size * sizeof(int));
    if (!copy->values) {
        memoryAllocationError();
    }
    memcpy(copy->values, mc->values, copy->size * sizeof(int));
    return copy;
}

void memoryCodeDestroy(MemoryCode mc) {
    free(mc->values);
    free(mc);
}

size_t memoryCodeGetSize(MemoryCode mc) {
    return mc->size;
}

void memoryCodeSetStartAddress(MemoryCode mc, int address) {
    mc->start_address = address;
}

int memoryCodeGetStartAddress(MemoryCode mc) {
    return mc->start_address;
}

size_t calcDirectiveDataSize(Statement s) {
    assert(statementGetType(s) == DIRECTIVE);

    const char *directive = statementGetMnemonic(s);

    if (strcmp(directive, DIRECTIVE_DATA) == 0) {
        return listLength(statementGetOperands(s));
    } else if (strcmp(directive, DIRECTIVE_STRING) == 0) {
        const char *str = listGetDataAt(statementGetOperands(s), 0);
        return strlen(str) - 2 + 1; // +1 for null terminator, -2 for quotes
    } else if (strcmp(directive, DIRECTIVE_STRUCT) == 0) {
        const char *str = listGetDataAt(statementGetOperands(s), 1);
        return 1 + strlen(str) - 2 + 1; // +1 for null terminator, -2 for quotes
    } else { // .entry or .extern
        return 0;
    }
}

void memoryCodeToObjFile(MemoryCode mc, FILE *f) {
    // TODO: implement
}
