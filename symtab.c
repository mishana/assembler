//
// Created by misha on 27/07/2022.
//

#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include "symtab.h"
#include "errors.h"


struct symtab_entry_t {
    char *name;
    int value;

    bool is_entry;
    // TODO: get rid of this
    bool is_struct;

    int line_num;
    SymbolType type;
};

/**
 * It creates a new symbol table entry with the given name and value.
 *
 * @param name The name of the symbol.
 * @param value The value of the symbol.
 * @param is_entry Whether the symbol is an entry or not.
 * @param is_struct Whether the symbol is a struct or not.
 * @param line_num The line number of the symbol.
 * @param type The type of the symbol.
 *
 * @return A new symbol table entry.
 */
SymtabEntry
symtabEntryCreate(const char *name, int value, bool is_entry, bool is_struct, int line_num, SymbolType type) {
    SymtabEntry e = malloc(sizeof(*e));
    if (!e) {
        memoryAllocationError();
    }

    e->name = strdup(name);
    e->value = value;
    e->is_entry = is_entry;
    e->line_num = line_num;
    e->type = type;
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
    return symtabEntryCreate(e->name, e->value, e->is_entry, e->is_struct, e->line_num, e->type);
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

/**
 * It checks if the symbol is extern.
 *
 * @param e The symbol table entry to check.
 */
bool symtabEntryIsEntry(SymtabEntry e) {
    return e->is_entry;
}

/**
 * It returns the line number of the symbol table entry.
 *
 * @param e The symbol table entry to get the line number of.
 */
int symtabEntryGetLineNum(SymtabEntry e) {
    return e->line_num;
}

/**
 * It returns the type of the symbol table entry.
 *
 * @param e The symbol table entry to get the type of.
 */
SymbolType symtabEntryGetType(SymtabEntry e) {
    return e->type;
}

/**
 * It checks if the entry is a struct.
 *
 * @param e The entry to check
 */
bool symtabEntryIsStruct(SymtabEntry e) {
    return e->is_struct;
}

/**
 * It sets the value of the symbol table entry.
 *
 * @param e The symbol table entry to set the value of.
 * @param value the value to set the entry to
 */
void symtabEntrySetValue(SymtabEntry e, int value) {
    e->value = value;
}


/**
 * It sets the is_entry flag of the symbol table entry.
 *
 * @param e The symbol table entry to set the is_entry flag for.
 * @param is_entry true if this is an entry point, false otherwise
 */
void symtabEntrySetIsEntry(SymtabEntry e, bool is_entry) {
    e->is_entry = is_entry;
}

/**
 * It finds the symbol table entry by name.
 *
 * @param symtab The symbol table to search.
 * @param name The name of the symbol to find.
 */
SymtabEntry symbolTableFindByName(List symtab, const char *name) {
    // This is a hack to get the entry in the symbol table
    SymtabEntry dummy_entry = symtabEntryCreate(name, 0, false, false, 0, SYMBOL_DATA);
    SymtabEntry found_entry;
    if (listFind(symtab, dummy_entry, (void **) &found_entry) == LIST_NOT_FOUND) {
        found_entry = NULL;
    }
    symtabEntryDestroy(dummy_entry);
    return found_entry;
}
