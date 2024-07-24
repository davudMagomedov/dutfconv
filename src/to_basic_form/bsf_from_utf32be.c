#include <stddef.h>
#include <stdint.h>

#include "../bsf_buffer.h"
#include "../error.h"

static uint32_t read_u32_from_char_iterator_be(struct char_iterator_s* iterator) {
    if (iterator->howfar < 4) {
        struct error_s err = {
            .kind = ERROR_KIND_FATAL,
            .message =
                "Could't read 4 bytes from char iterator. Maybe there's problem in the file."};
        error_stack_push(err);
    }

    uint32_t byte_1 = iterator->finger[0], byte_2 = iterator->finger[1],
             byte_3 = iterator->finger[2], byte_4 = iterator->finger[3];
    iterator->howfar -= 4;

    return (byte_1 << 24) + (byte_2 << 16) + (byte_3 << 8) + (byte_4);
}

struct bsf_static_string_s bsf_from_utf32be(struct char_iterator_s iterator) {
    struct bsf_buffer_s buffer = bsf_buffer_init();

    while (iterator.howfar) {
        uint32_t unicode = read_u32_from_char_iterator_be(&iterator);
        bsf_buffer_push(&buffer, unicode);
    }

    return bsf_buffer_to_static_string(buffer);
}
