#include <stdlib.h>

#include "../basic_form.h"
#include "../char_buffer.h"

struct char_slice_s utf32le_from_bsf_static_string(struct bsf_static_string_s static_string) {
    struct char_slice_s slice = {
        .data = realloc(static_string.data, sizeof(uint32_t) * static_string.len),
        .len = sizeof(uint32_t) * static_string.len};

    return slice;
}
