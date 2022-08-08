//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_MACRO_H
#define ASSEMBLER_MACRO_H


typedef struct macro_t *Macro;

Macro macroCreate(const char *name, const char *body);

int macroCmp(Macro m1, Macro m2);

Macro macroCopy(Macro m);

void macroDestroy(Macro m);

const char *macroGetBody(Macro m);

const char *macroGetName(Macro m);

#endif //ASSEMBLER_MACRO_H
