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

    bool is_extern;
    bool is_struct;

    int line_num;
    SymbolType type;
};

/**
 * It creates a new symbol table entry with the given name and value.
 *
 * @param name The name of the symbol.
 * @param value The value of the symbol.
 * @param is_extern Whether the symbol is external or not.
 * @param is_struct Whether the symbol is a struct or not.
 * @param line_num The line number of the symbol.
 * @param type The type of the symbol.
 *
 * @return A new symbol table entry.
 */
SymtabEntry
symtabEntryCreate(const char *name, int value, bool is_extern, bool is_struct, int line_num, SymbolType type) {
    SymtabEntry e = malloc(sizeof(*e));
    if (!e) {
        memoryAllocationError();
    }

    e->name = strdup(name);
    e->value = value;
    e->is_extern = is_extern;
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
    return symtabEntryCreate(e->name, e->value, e->is_extern, e->is_struct, e->line_num, e->type);
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
bool symtabEntryIsExtern(SymtabEntry e) {
    return e->is_extern;
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
 * It checks if the name is in the symbol table.
 *
 * @param symtab The symbol table to search in.
 * @param name the name of the symbol to look for
 */
bool isInSymbolTable(List symtab, const char *name) {
    bool res;
    SymtabEntry dummy_entry = symtabEntryCreate(name, 0, false, false, 0, SYMBOL_DATA);
    SymtabEntry found_entry;
    if (listFind(symtab, dummy_entry, (void **) &found_entry) == LIST_SUCCESS) {
        res = true;
    } else {
        res = false;
    }
    symtabEntryDestroy(dummy_entry);
    symtabEntryDestroy(found_entry);

    return res;
}

bool isStructInSymbolTable(List symtab, const char *name) {
    bool res;
    SymtabEntry dummy_entry = symtabEntryCreate(name, 0, false, true, 0, SYMBOL_DATA);
    SymtabEntry found_entry;
    if (listFind(symtab, dummy_entry, (void **) &found_entry) == LIST_SUCCESS) {
        res = symtabEntryIsStruct(found_entry);
    } else {
        res = false;
    }
    symtabEntryDestroy(dummy_entry);
    symtabEntryDestroy(found_entry);

    return res;
}

void symtabEntrySetValue(SymtabEntry e, int value) {
    e->value = value;
}
