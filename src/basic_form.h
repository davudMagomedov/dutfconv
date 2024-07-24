#pragma once

#include <stddef.h>
#include <stdint.h>

#include "char_buffer.h"
#include "char_iterator.h"

typedef uint32_t* basic_string_form_t;

// Drop: `free(static_string.data)`.
//
// Validity conditions:
// 1. `data` is a pointer pointing to a allocated memory area of size `len`.
// 2. `data` may be any value if `len == 0`. Then, `data` is just ignored.
struct bsf_static_string_s {
    basic_string_form_t data;
    size_t len;
};

struct bsf_static_string_s bsf_from_utf8(struct char_iterator_s);
struct bsf_static_string_s bsf_from_utf16le(struct char_iterator_s);
struct bsf_static_string_s bsf_from_utf16be(struct char_iterator_s);
struct bsf_static_string_s bsf_from_utf32le(struct char_iterator_s);
struct bsf_static_string_s bsf_from_utf32be(struct char_iterator_s);

struct char_slice_s utf8_from_bsf_static_string(struct bsf_static_string_s);
struct char_slice_s utf16le_from_bsf_static_string(struct bsf_static_string_s);
struct char_slice_s utf16be_from_bsf_static_string(struct bsf_static_string_s);
struct char_slice_s utf32le_from_bsf_static_string(struct bsf_static_string_s);
struct char_slice_s utf32be_from_bsf_static_string(struct bsf_static_string_s);
