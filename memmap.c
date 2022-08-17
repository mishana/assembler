//
// Created by misha on 27/07/2022.
//

#include <string.h>
#include <assert.h>
#include "memmap.h"
#include "const_tables.h"


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
