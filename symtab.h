//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_SYMTAB_H
#define ASSEMBLER_SYMTAB_H

#include "linkedlist.h"

#define SYMBOL_ADDRESS_NOT_FOUND -1

typedef enum SymbolType {
    SYMBOL_DATA,
    SYMBOL_CODE,
    SYMBOL_EXTERN
} SymbolType;

typedef struct symtab_entry_t *SymtabEntry;

SymtabEntry symtabEntryCreate(const char *name, int value, bool is_entry, bool is_struct, int line_num, SymbolType type);

int symtabEntryCmp(SymtabEntry e1, SymtabEntry e2);

SymtabEntry symtabEntryCopy(SymtabEntry e);

void symtabEntryDestroy(SymtabEntry e);

const char *symtabEntryGetName(SymtabEntry e);

int symtabEntryGetValue(SymtabEntry e);

bool symtabEntryIsEntry(SymtabEntry e);

int symtabEntryGetLineNum(SymtabEntry e);

SymbolType symtabEntryGetType(SymtabEntry e);

void symtabEntrySetValue(SymtabEntry e, int value);

void symtabEntrySetIsEntry(SymtabEntry e, bool is_entry);

int symbolTableGetAddress(List symtab, const char *name);

SymtabEntry symbolTableFindByName(List symtab, const char *name);

#endif //ASSEMBLER_SYMTAB_H
