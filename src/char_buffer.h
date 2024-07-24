#pragma once

#include <stdlib.h>

struct char_slice_s {
    char* data;
    size_t len;
};

struct char_buffer_s {
    char* buffer;
    size_t len;
    size_t capacity;
};

const struct char_buffer_s char_buffer_init();
void char_buffer_push(struct char_buffer_s*, char);
struct char_slice_s char_buffer_to_slice(struct char_buffer_s);
// Assumes `self->len == self.capacity`. Otherwise - UB.
//
// Gives guarantees that after this function `self->len < self->capacity`.
void char_buffer_grow(struct char_buffer_s*);
