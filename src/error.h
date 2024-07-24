#pragma once

#include <stddef.h>

#define ERROR_STACK_SIZE 512

enum error_kind_e {
    ERROR_KIND_FATAL,
    ERROR_KIND_HARMLESS,
    ERROR_KIND_WARNING,
};

struct error_s {
    enum error_kind_e kind;
    const char* message;
};

struct error_stack_s {
    struct error_s data[ERROR_STACK_SIZE];
    size_t finger;
};

/* `void error_stack_pus(
 *     struct error_s error,
 * )`
 *
 * Pushes given `error` to the error stack. If given `error` is fatal (iow when
 * `error.kind == ERROR_KIND_FATAL`) the function finishes the program with
 * printing each error. If the stack is fuill, the function also finishes
 * program with printing each error.
 * */
void error_stack_push(struct error_s);

/* `void error_stack_untwist()`
 *
 * Untwists the error stack printing each error not finishing the program.
 * */
void error_stack_untwist();
