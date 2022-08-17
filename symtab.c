//
// Created by misha on 27/07/2022.
//

#include <malloc.h>
#include <string.h>
#include "symtab.h"


struct symtab_entry_t {
    char *name;
    int value;
};

/**
 * It creates a new symbol table entry with the given name and value.
 *
 * @param name The name of the symbol.
 * @param value The value of the symbol.
 */
SymtabEntry symtabEntryCreate(const char *name, int value) {
    SymtabEntry e = malloc(sizeof(*e));
    e->name = strdup(name);
    e->value = value;
    return e;
}

/**
 * It compares two entries in the symbol table.
 *
 * @param e1 The first entry to compare
 * @param e2 The second entry to compare.
 */
int symtabEntryCmp(SymtabEntry e1, SymtabEntry e2) {
    return strcmp(e1->name, e2->name);
}

/**
 * It copies the symbol table entry.
 *
 * @param e The entry to copy.
 */
SymtabEntry symtabEntryCopy(SymtabEntry e) {
    return symtabEntryCreate(e->name, e->value);
}

/**
 * It destroys the symbol table entry.
 *
 * @param e The entry to destroy.
 */
void symtabEntryDestroy(SymtabEntry e) {
    free((void *) e->name);
    free(e);
}

/**
 * It returns the name of the symbol table entry.
 *
 * @param e The symbol table entry to get the name of.
 */
const char *symtabEntryGetName(SymtabEntry e) {
    return e->name;
}

/**
 * It returns the value of the symbol table entry.
 *
 * @param e The symbol table entry to get the value of.
 */
int symtabEntryGetValue(SymtabEntry e) {
    return e->value;
}
