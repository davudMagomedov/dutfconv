#pragma once

#include <stddef.h>

/* Iterator through a sequence of chars.
 *
 * After `char_iterator_next` call that set `finger` to `0`, an iterator becomes invalid.
 *
 * Invariants:
 * - `howfar != 0` if and only if `howfar != NULL`.
 * */
struct char_iterator_s {
    const char* finger;
    size_t howfar;
};

/* Returns next char.
 *
 * When `self->howfar == 0` it returns garbage. */
char char_iterator_next(struct char_iterator_s*);
