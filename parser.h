#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include <stdbool.h>

#include "linkedlist.h"


typedef enum {
    DIRECTIVE, INSTRUCTION, MACRO_START, MACRO_END, COMMENT, EMPTY_LINE, OTHER
} StatementType;

typedef struct statement_t *Statement;

Statement
parse(const char *line, int line_num, const char *filename, const char *filename_suffix, bool is_pre_assembly);

Statement
statementCreate(int line_num, StatementType type, const char *raw_text, const char *label, const char *mnemonic, List operands,
                List tokens);

void statementDestroy(Statement s);

StatementType statementGetType(Statement s);

const char *statementGetLabel(Statement s);

const char *statementGetMnemonic(Statement s);

const List statementGetOperands(Statement s);

const List statementGetTokens(Statement s);

int statementGetLineNum(Statement s);

bool isDataStoreDirective(const char *directive);


#endif
