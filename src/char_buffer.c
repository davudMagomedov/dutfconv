#include "char_buffer.h"

const struct char_buffer_s char_buffer_init() {
    struct char_buffer_s buffer = {.buffer = NULL, .len = 0, .capacity = 0};
    return buffer;
}

void char_buffer_push(struct char_buffer_s* self, char ch) {
    if (self->capacity == self->len) {
        char_buffer_grow(self);
    }

    self->buffer[self->len++] = ch;
}

struct char_slice_s char_buffer_to_slice(struct char_buffer_s self) {
    struct char_slice_s slice = {.len = self.len};

    if (self.capacity) {
        // FIX: Case when `realloc` returns `0`.
        slice.data = realloc(self.buffer, sizeof(char) * self.len);
    } else {
        slice.data = NULL;
    }

    return slice;
}

void char_buffer_grow(struct char_buffer_s* self) {
    if (self->capacity == 0) {
        // FIX: Case when `malloc` returns `0`.
        self->buffer = malloc(sizeof(char));
        self->capacity = 1;
        return;
    }

    // FIX: Case when `realloc` returns `0`.
    self->buffer = realloc(self->buffer, sizeof(char) * (self->capacity *= 2));
}
