#include <malloc.h>
#include <string.h>

#include "macro.h"
#include "errors.h"
#include "str_utils.h"


/* Defining a new type called `struct macro_t` which is a struct with two fields: `name` and `body`. */
struct macro_t {
    int def_line_num;

    const char *name;
    const char *body;
};

/**
 * It creates a macro.
 *
 * @param name The name of the macro.
 * @param body The body of the macro.
 */
Macro macroCreate(const char *name, const char *body, int def_line_num) {
    Macro m = (Macro) malloc(sizeof(*m));
    if (!m)
        memoryAllocationError();

    m->name = strdup(name);
    if (!m->name)
        memoryAllocationError();

    if (body) {
        m->body = strdup(body);
        if (!m->body)
            memoryAllocationError();
    } else {
        m->body = NULL;
    }

    m->def_line_num = def_line_num;

    return m;
}

/**
 * It compares two macros and returns the result.
 *
 * @param m1 The first macro to compare.
 * @param m2 The macro to compare to.
 */
int macroCmp(Macro m1, Macro m2) {
    return strcmp(m1->name, m2->name);
}

/**
 * It copies the macro m and returns the copy.
 *
 * @param m The macro to copy.
 */
Macro macroCopy(Macro m) {
    return macroCreate(m->name, m->body, m->def_line_num);
}

/**
 * It destroys the macro.
 *
 * @param m The macro to destroy.
 */
void macroDestroy(Macro m) {
    if (m->name)
        free((char *) m->name);
    if (m->body)
        free((char *) m->body);
    free(m);
}

/**
 * It returns the body of the macro.
 *
 * @param m The macro to get the body of.
 */
const char *macroGetBody(Macro m) {
    return m->body;
}

/**
 * It returns the name of the macro.
 *
 * @param m The macro to get the name of.
 */
const char *macroGetName(Macro m) {
    return m->name;
}

/**
 * It returns the line number of the macro definition.
 *
 * @param m The macro to get the line number of.
 */
int macroGetDefLineNum(Macro m) {
    return m->def_line_num;
}
