#include "../basic_form.h"
#include "../char_buffer.h"

/* Parses given unicode symbol to utf16 sequence. Returns count of written 16-bit-symbols (max - 2).
 * It means that given buffer (in second argument) should be greater than or equal to 2 in size. */
static size_t utf16be_char_from_unicode(uint32_t unicode, uint8_t* bytes) {
    if (unicode < 0x10000) {
        // assert(unicode == unicode & 0b1111_1111_1111_1111)
        // bytes[0] = unicode[8..16]
        // bytes[1] = unicode[0..8]
        bytes[0] = unicode >> 8;
        bytes[1] = unicode & 0b11111111;
        return 2;
    } else {
        uint32_t unicode_lower_16bits = unicode - 0x10000;
        uint16_t lower_10 = (uint16_t)(unicode_lower_16bits & 0x3FF);
        uint16_t higher_10 = (uint16_t)(unicode_lower_16bits << 10);

        uint16_t first_word = higher_10 | 0xD800;
        uint16_t second_word = lower_10 | 0xDC00;

        // bytes[0] = first_word[8..16]
        // bytes[1] = first_word[0..8]
        // bytes[2] = second_word[8..16]
        // bytes[3] = second_word[0..8]
        bytes[0] = first_word >> 8;
        bytes[1] = first_word & 0b11111111;
        bytes[2] = second_word >> 8;
        bytes[3] = second_word & 0b11111111;

        return 4;
    }
}

struct char_slice_s utf16be_from_bsf_static_string(struct bsf_static_string_s static_string) {
    uint32_t* unicode_chars = static_string.data;
    size_t howfar = static_string.len;
    struct char_buffer_s char_buffer = char_buffer_init();

    while (howfar) {
        uint32_t unicode = *(unicode_chars++);
        uint8_t bytes[4];

        size_t written_count = utf16be_char_from_unicode(unicode, bytes);

        char_buffer_push(&char_buffer, bytes[0]);
        char_buffer_push(&char_buffer, bytes[1]);
        if (written_count == 4) {
            char_buffer_push(&char_buffer, bytes[2]);
            char_buffer_push(&char_buffer, bytes[3]);
        }

        howfar--;
    }

    return char_buffer_to_slice(char_buffer);
}
