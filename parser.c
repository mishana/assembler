//
// Created by misha on 30/07/2022.
//

#include "parser.h"


struct parsed_statement_t {
    const char *label;
    const char *mnemonic;
    List *operands;
};

struct statement_t{
    StatementType type;
    const char *raw_text;
    ParsedStatement *parsed_statement;
};

Statement parse(const char *line) {

}
