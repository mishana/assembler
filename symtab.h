//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_SYMTAB_H
#define ASSEMBLER_SYMTAB_H

typedef enum SymbolType {
    SYMBOL_DATA,
    SYMBOL_CODE,
    SYMBOL_EXTERN
} SymbolType;

typedef struct symtab_entry_t *SymtabEntry;

SymtabEntry symtabEntryCreate(const char *name, int value, bool is_extern, int line_num, SymbolType type);

int symtabEntryCmp(SymtabEntry e1, SymtabEntry e2);

SymtabEntry symtabEntryCopy(SymtabEntry e);

void symtabEntryDestroy(SymtabEntry e);

const char *symtabEntryGetName(SymtabEntry e);

int symtabEntryGetValue(SymtabEntry e);

bool symtabEntryIsExtern(SymtabEntry e);

int symtabEntryGetLineNum(SymtabEntry e);

SymbolType symtabEntryGetType(SymtabEntry e);

#endif //ASSEMBLER_SYMTAB_H
