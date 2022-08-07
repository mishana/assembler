//
// Created by misha on 30/07/2022.
//

#include "parser.h"
#include "str_utils.h"
#include "errors.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define WHITESPACE_DELIM "\t\n "
#define OPERANDS_DELIM ","
#define LABEL_SUFFUX ":"
#define COMMENT_PREFIX ";"


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

static bool isCommentLine(const char *line) {
    return strStartsWith(line, COMMENT_PREFIX, true);
}

static bool isLabel(const char *s) {
    // TODO: check label validity
    return strEndsWith(s, LABEL_SUFFUX);
}

static bool isDirective(const char *s) {
    // TODO: check validity
    // TODO
    return false;
}

static bool isInstruction(const char *s) {
    // TODO: check validity
    // TODO
    return false;
}

static bool isMacroStart(const char *s) {
    // TODO: check validity
    // TODO
    return false;
}

static bool isMacroEnd(const char *s) {
    // TODO: check validity
    // TODO
    return false;
}

Statement parse(const char *line) {
    if (isCommentLine(line)) {
        return NULL;
    }

    const char *line_replaced = strReplace(line, OPERANDS_DELIM, WHITESPACE_DELIM);
    if (!line_replaced) {
        memoryAllocationError();
    }

    List tokens = strSplit(line_replaced, WHITESPACE_DELIM);
    /* Checking that the list of tokens is not empty. */
    assert(listLength(tokens) > 0);

    free((char *)line_replaced);

    Node token_node = listHead(tokens);
    const char *token = nodeGetData(token_node);

    char *label = NULL;
    if (isLabel(token)) {
        label = strdup(token);

        token_node = nodeGetNext(token_node);
        // TODO: check if not NULL
        token = nodeGetData(token_node);
    }
    // TODO: continue, if macro start, macro end, isDirective, isInstruction, check validity...


    // TODO
    listDestroy(tokens);
    return NULL;
}
