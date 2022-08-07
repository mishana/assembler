//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include <stdbool.h>

#include "linkedlist.h"


typedef enum {
    DIRECTIVE, INSTRUCTION, MACRO_START, MACRO_END, MACRO_REF
} StatementType;

typedef struct parsed_statement_t *ParsedStatement;

typedef struct statement_t *Statement;

// TODO implement init, destroy, parse...
Statement parse(const char *line);

#endif //ASSEMBLER_PARSER_H
