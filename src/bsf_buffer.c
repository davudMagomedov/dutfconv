#include <stddef.h>
#include <stdlib.h>

#include "basic_form.h"
#include "bsf_buffer.h"

const struct bsf_buffer_s bsf_buffer_init() {
    struct bsf_buffer_s buffer = {.buffer = 0, .len = 0, .capacity = 0};
    return buffer;
}

void bsf_buffer_push(struct bsf_buffer_s* self, uint32_t ch) {
    if (self->capacity == self->len) {
        bsf_buffer_grow(self);
    }

    self->buffer[self->len++] = ch;
}

struct bsf_static_string_s bsf_buffer_to_static_string(struct bsf_buffer_s self) {
    struct bsf_static_string_s static_string = {.len = self.len};
    if (self.capacity)
        // FIX: Case when `realloc` returns `0`.
        static_string.data = realloc(self.buffer, sizeof(uint32_t) * self.len);
    else {
        static_string.data = NULL;
    }

    return static_string;
}

void bsf_buffer_grow(struct bsf_buffer_s* self) {
    if (self->capacity == 0) {
        // FIX: Case when `malloc` returns `0`.
        self->buffer = malloc(sizeof(uint32_t));
        self->capacity = 1;
        return;
    }

    // FIX: Case when `realloc` returns `0`.
    self->buffer = realloc(self->buffer, sizeof(uint32_t) * (self->capacity *= 2));
}
