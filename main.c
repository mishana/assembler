#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "errors.h"
#include "pre_assembly.h"
#include "first_pass.h"
#include "second_pass.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        errorWithMsg("Not enough arguments! Need to specify files to compile (without suffix).");
    }

    for (int i = 1; i < argc; ++i) {
        const char *file_to_compile = argv[i];

        printf("Run pre-assembly for %s\n", file_to_compile);
        bool pre_assembly_res = run_pre_assembly(file_to_compile);
        if (!pre_assembly_res) {
            errorWithMsg("Pre-assembly failed. exiting");
        }

        printf("Run first-pass for %s\n", file_to_compile);
        List symtab, machine_codes, memory_codes;
        bool res = run_first_pass(file_to_compile, &symtab, &machine_codes, &memory_codes);
        if (!res) {
            printf("First-pass for %s failed. skipping second-pass\n", file_to_compile);
            listDestroy(symtab);
            listDestroy(machine_codes);
            listDestroy(memory_codes);
            continue;
        }

        printf("Run second-pass for %s\n", file_to_compile);
        run_second_pass(file_to_compile, symtab, machine_codes, memory_codes);
        printf("=============================================================");
    }

    return 0;
}
