//
// Created by misha on 27/07/2022.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "pre_assembly.h"
#include "linkedlist.h"
#include "str_utils.h"
#include "errors.h"
#include "macro.h"

#define SOURCE_FILE_SUFFIX ".as"
#define AFTER_MACRO_SUFFIX ".am"

#define MAX_FILENAME_LEN 1000
#define MAX_LINE_LEN 80 + 1


void concat_filename_and_suffix(char *filename_with_suffix, const char *filename, const char *suffix) {
    int filename_with_suffix_len = (int) strlen(filename) + (int) strlen(suffix);
    /* Concatenating the filename with the suffix. */
    snprintf(filename_with_suffix, filename_with_suffix_len + 1, "%s%s", filename, suffix);
}

void unfold_macros(FILE *src_file, FILE *dst_file) {
    List macros = listCreate((list_eq) macroCmp, (list_copy) macroCopy, (list_free) macroDestroy);

    bool is_macro = false;
    char *macro_name;
    char *macro_body = NULL;

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, src_file) != NULL) {
        if (startsWith(line, START_MACRO, true)) { // macro statement
            is_macro = true;

            // start of macro definition
            int macro_definition_idx = findNextNonWhitespace(line, 0);
            // start of macro name
            int macro_name_start_idx = findNextNonWhitespace(line, macro_definition_idx + strlen(START_MACRO));
            // end of macro name
            int macro_name_end_idx = findNextWhitespace(line, macro_name_start_idx);

            macro_name = strndup(line + macro_name_start_idx, macro_name_end_idx - macro_name_start_idx);
            if (!macro_name)
                memoryAllocationError();

            // TODO: init macro body
//            macro_body = strndup(line + macro_name_start_idx, macro_name_end_idx - macro_name_start_idx);
//            if (!macro_body)
//                memoryAllocationError();


        } else if (startsWith(line, END_MACRO, true)) { // endmacro statement
            Macro m = macroCreate(macro_name, macro_body);
            listAppend(macros, m);
            macroDestroy(m);

            is_macro = false;
            free(macro_name);
            free(macro_body);

        } else if (is_macro) {
            // TODO: append to macro body
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
            /* It's getting the first word in the line. */
            int first_word_start_idx = findNextNonWhitespace(line, 0);
            int first_word_end_idx = findNextWhitespace(line, first_word_start_idx);
            // mark the end of the first word
            char *first_word = strndup(line + first_word_start_idx, first_word_end_idx - first_word_start_idx);

            Macro m = macroCreate(first_word, NULL);
            Macro predefined_macro = listFind(macros, m);
            if (predefined_macro != NULL)
                fputs(macroGetBody(predefined_macro), dst_file);
            else
                fputs(line, dst_file);
        }
    }
    listDestroy(macros);
}

void run_pre_assembly(const char *filename) {
    char filename_with_suffix[MAX_FILENAME_LEN];

    concat_filename_and_suffix(filename_with_suffix, filename, SOURCE_FILE_SUFFIX);
    FILE *src_file = fopen(filename_with_suffix, "r");

    concat_filename_and_suffix(filename_with_suffix, filename, AFTER_MACRO_SUFFIX);
    FILE *dst_file = fopen(filename_with_suffix, "w");

    unfold_macros(src_file, dst_file);

    fclose(src_file);
    fclose(dst_file);
}