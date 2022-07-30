//
// Created by misha on 30/07/2022.
//

#include <malloc.h>
#include <string.h>

#include "macro.h"
#include "errors.h"


/* Defining a new type called `struct macro_t` which is a struct with two fields: `name` and `body`. */
struct macro_t {
    const char *name;
    const char *body;
};

/**
 * It creates a macro.
 *
 * @param name The name of the macro.
 * @param body The body of the macro.
 */
Macro macroCreate(const char *name, const char *body) {
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
    }

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
    return macroCreate(m->name, m->body);
}

/**
 * It destroys the macro.
 *
 * @param m The macro to destroy.
 */
void macroDestroy(Macro m) {
    free((char *) m->name);
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