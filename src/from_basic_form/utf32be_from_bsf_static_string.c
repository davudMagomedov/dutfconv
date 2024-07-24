#include <stdlib.h>

#include "../basic_form.h"
#include "../char_buffer.h"

static uint32_t from_le_to_be(uint32_t unicode) {
    return ((unicode & 0b11111111) << 24) + ((unicode >> 8 & 0b11111111) << 16) +
           ((unicode >> 16 & 0b11111111) << 8) + (unicode >> 24 & 0b11111111);
}

struct char_slice_s utf32be_from_bsf_static_string(struct bsf_static_string_s static_string) {
    struct char_slice_s slice = {
        .data = realloc(static_string.data, sizeof(uint32_t) * static_string.len),
        .len = sizeof(uint32_t) * static_string.len};

    return slice;
}
