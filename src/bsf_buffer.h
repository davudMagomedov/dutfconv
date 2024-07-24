#pragma once

#include "basic_form.h"

struct bsf_buffer_s {
    basic_string_form_t buffer;
    size_t len;
    size_t capacity;
};

const struct bsf_buffer_s bsf_buffer_init();
void bsf_buffer_push(struct bsf_buffer_s*, uint32_t);
struct bsf_static_string_s bsf_buffer_to_static_string(struct bsf_buffer_s);
// Assumes `self->len == self.capacity`. Otherwise - UB.
//
// Gives guarantees that after this function `self->len < self->capacity`.
void bsf_buffer_grow(struct bsf_buffer_s*);
