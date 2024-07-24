#include <stddef.h>
#include <stdint.h>

#include "../bsf_buffer.h"
#include "../error.h"

static uint32_t read_u32_from_char_iterator_le(struct char_iterator_s* iterator) {
    if (iterator->howfar < 4) {
        struct error_s err = {
            .kind = ERROR_KIND_FATAL,
            .message =
                "Could't read 4 bytes from char iterator. Maybe there's problem in the file."};
        error_stack_push(err);
    }

    iterator->howfar -= 4;
    uint32_t value = *(uint32_t*)iterator->finger;
    iterator->finger += 4;

    return value;
}

struct bsf_static_string_s bsf_from_utf32le(struct char_iterator_s iterator) {
    struct bsf_buffer_s buffer = bsf_buffer_init();

    while (iterator.howfar) {
        uint32_t unicode = read_u32_from_char_iterator_le(&iterator);
        bsf_buffer_push(&buffer, unicode);
    }

    return bsf_buffer_to_static_string(buffer);
}
