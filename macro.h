#ifndef ASSEMBLER_MACRO_H
#define ASSEMBLER_MACRO_H


typedef struct macro_t *Macro;

Macro macroCreate(const char *name, const char *body, int def_line_num);

int macroCmp(Macro m1, Macro m2);

Macro macroCopy(Macro m);

void macroDestroy(Macro m);

const char *macroGetBody(Macro m);

const char *macroGetName(Macro m);

int macroGetDefLineNum(Macro m);

#endif
