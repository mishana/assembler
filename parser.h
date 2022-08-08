//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include <stdbool.h>

#include "linkedlist.h"


typedef enum {
    DIRECTIVE, INSTRUCTION, MACRO_START, MACRO_END, OTHER
} StatementType;

typedef struct statement_t *Statement;

Statement parse(const char *line);

Statement
statementCreate(StatementType type, const char *raw_text, const char *label, const char *mnemonic, List operands,
                List tokens);

void statementDestroy(Statement s);

StatementType statementGetType(Statement s);

const char *statementGetRawText(Statement s);

const char *statementGetLabel(Statement s);

const char *statementGetMnemonic(Statement s);

const List statementGetOperands(Statement s);

const List statementGetTokens(Statement s);

#endif //ASSEMBLER_PARSER_H
