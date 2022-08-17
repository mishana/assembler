//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_SYMTAB_H
#define ASSEMBLER_SYMTAB_H

typedef struct symtab_entry_t *SymtabEntry;

SymtabEntry symtabEntryCreate(const char *name, int value);

int symtabEntryCmp(SymtabEntry e1, SymtabEntry e2);

SymtabEntry symtabEntryCopy(SymtabEntry e);

void symtabEntryDestroy(SymtabEntry e);

const char *symtabEntryGetName(SymtabEntry e);

int symtabEntryGetValue(SymtabEntry e);

#endif //ASSEMBLER_SYMTAB_H
