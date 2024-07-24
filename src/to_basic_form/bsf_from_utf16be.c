#include <stddef.h>
#include <stdint.h>

#include "../bsf_buffer.h"
#include "../error.h"

static uint16_t read_16word_be(struct char_iterator_s* iterator) {
    if (iterator->howfar < 2) {
        struct error_s err = {
            .kind = ERROR_KIND_FATAL,
            .message =
                "Couldn't read 2 bytes from char iterator. Maybe there's problem in the file.",
        };
        error_stack_push(err);
    }

    uint16_t word =
        ((uint16_t)char_iterator_next(iterator) << 8) + (uint16_t)char_iterator_next(iterator);

    return word;
}

static uint32_t read_unicode_utf16be(struct char_iterator_s* iterator) {
    uint16_t leading, trailing;

    leading = read_16word_be(iterator);
    if (leading < 0xD800 || leading > 0xDFFF) {
        return (uint32_t)leading;
    } else if (leading >= 0xDC00) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "Unexpected behaviour during converting."};
        error_stack_push(err);
    }

    uint32_t code = (uint32_t)(leading & 0x3FF) << 10;

    trailing = read_16word_be(iterator);
    if (trailing < 0xDC00 || trailing > 0xDFFF) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "Unexpected behaviour during converting."};
        error_stack_push(err);
    } else {
        code = code | (uint32_t)(trailing & 0x3FF);
        return code + 0x10000;
    }
}

struct bsf_static_string_s bsf_from_utf16be(struct char_iterator_s iterator) {
    struct bsf_buffer_s buffer = bsf_buffer_init();

    while (iterator.howfar) {
        uint32_t unicode = read_unicode_utf16be(&iterator);
        bsf_buffer_push(&buffer, unicode);
    }

    return bsf_buffer_to_static_string(buffer);
}
