//
// Created by misha on 30/07/2022.
//

#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ALLOCATION_ERROR -2


void memoryAllocationError() {
    printf("Memory Allocation ERROR :(");
    exit(MEMORY_ALLOCATION_ERROR);
}

void errorWithMsg(const char *msg) {
    printf("%s", msg);
    exit(-1);
}
