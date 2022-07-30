#include <stdio.h>
#include <stdlib.h>

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
        run_pre_assembly(file_to_compile);
        printf("Run first-pass for %s\n", file_to_compile);
        run_first_pass(file_to_compile);
        printf("Run second-pass for %s\n", file_to_compile);
        run_second_pass(file_to_compile);
        printf("=============================================================");
    }

    return 0;
}
