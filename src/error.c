#include <stdio.h>
#include <stdlib.h>

#include "error.h"

struct error_stack_s error_stack = {.finger = 0};

static void print_error(const struct error_s* error) {
    switch (error->kind) {
    case ERROR_KIND_WARNING:
        printf("[WARNING] ");
        break;
    case ERROR_KIND_HARMLESS:
        printf("[HARMLESS] ");
        break;
    case ERROR_KIND_FATAL:
        printf("[FATAL] ");
        break;
    }

    printf("%s\n", error->message);
}

void error_stack_push(struct error_s error) {
    error_stack.data[error_stack.finger++] = error;

    if (error.kind == ERROR_KIND_FATAL || error_stack.finger == ERROR_STACK_SIZE) {
        error_stack_untwist();
        exit(EXIT_FAILURE);
    }
}

void error_stack_untwist() {
    for (size_t error_i = 0; error_i < error_stack.finger; ++error_i) {
        print_error(&error_stack.data[error_i]);
    }

    error_stack.finger = 0;
}
