//
// Created by misha on 27/07/2022.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "pre_assembly.h"
#include "linkedlist.h"
#include "str_utils.h"
#include "errors.h"
#include "macro.h"
#include "parser.h"


#define SOURCE_FILE_SUFFIX ".as"
#define AFTER_MACRO_SUFFIX ".am"

#define MAX_LINE_LEN 2048


void unfold_macros(FILE *src_file, FILE *dst_file) {
    List macros = listCreate((list_eq) macroCmp, (list_copy) macroCopy, (list_free) macroDestroy);

    bool is_macro = false;
    const char *macro_name;
    char *macro_body = NULL;

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, src_file) != NULL) {
        // TODO: what about validity errors??
        /* It's parsing the line. */
        Statement s = parse(line);
        if (!s) { // comment or empty line
            continue;
        }

        if (statementGetType(s) == MACRO_START) {
            is_macro = true;
            macro_name = strdup(listGetDataAt(statementGetOperands(s), 0));
            // TODO: check validity of macro name and validity of line

        } else if (statementGetType(s) == MACRO_END) {
            Macro m = macroCreate(macro_name, macro_body);
            listAppend(macros, m);
            macroDestroy(m);

            is_macro = false;
            free((void *)macro_name);
            free(macro_body);

        } else if (is_macro) { // inside macro - append to macro body
            char *tmp = macro_body;

            if (!macro_body) {
                /* It's appending the line to the macro body. */
                if (asprintf(&macro_body, "%s", line) == -1)
                    memoryAllocationError();
            } else {
                /* It's appending the line to the macro body. */
                if (asprintf(&macro_body, "%s%s", macro_body, line) == -1)
                    memoryAllocationError();
                free(tmp);
            }

        } else { // outside macro definition, check if referencing macro that needs unfolding
            const char *first_word = listGetDataAt(statementGetTokens(s), 0);

            /* It's creating a dummy macro with the name of the first word in the line. */
            Macro dummy = macroCreate(first_word, NULL);
            Macro found_macro;
            ListResult res = listFind(macros, dummy, (void **) &found_macro);
            macroDestroy(dummy);

            if (res == LIST_FOUND) {
                fputs(macroGetBody(found_macro), dst_file);
                macroDestroy(found_macro);
            }
            else {
                fputs(line, dst_file);
            }
        }
        statementDestroy(s);
    }
    listDestroy(macros);
}

void run_pre_assembly(const char *filename) {
    const char *filename_with_suffix = strConcat(filename, SOURCE_FILE_SUFFIX);
    FILE *src_file = fopen(filename_with_suffix, "r");
    if (!src_file) {
        fileNotFoundError(filename_with_suffix);
    }
    free((void *) filename_with_suffix);

    filename_with_suffix = strConcat(filename, AFTER_MACRO_SUFFIX);
    FILE *dst_file = fopen(filename_with_suffix, "w");
    if (!dst_file) {
        fileNotFoundError(filename_with_suffix);
    }
    free((void *) filename_with_suffix);

    unfold_macros(src_file, dst_file);

    fclose(src_file);
    fclose(dst_file);
}
