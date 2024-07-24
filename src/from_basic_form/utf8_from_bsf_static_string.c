#include "../basic_form.h"
#include "../char_buffer.h"

/* Parses unicode word (first argument) into utf8 sequence (second argument).
 *
 * Returns count of written bytes. If given unicode word is wrong, returns 0. Returning number is
 * less than 5 so you need to put array with size >= 4 in the second argument. */
static size_t utf8_char_from_unicode(uint32_t unicode, uint8_t* bytes) {
    if (unicode < 0x80) { // unicode[0..7]
        // bytes[0] = unicode[7 lower bits]
        bytes[0] = (uint8_t)(unicode & 0b1111111);
        return 1;
    } else if (unicode < 0x800) { // unicode[0..11]
        // bytes[0] = u8(unicode[6..11] | 0b11000000)
        // bytes[1] = u8(unicode[0..6]  | 0b1000000)
        bytes[0] = (uint8_t)(unicode >> 6) | 0b11000000;
        bytes[1] = (uint8_t)(unicode & 0b111111) | 0b10000000;
        return 2;
    } else if (unicode < 0x10000) { // unicode[0..16]
        // bytes[0] = u8(unicode[12..16] | 0b11100000)
        // bytes[1] = u8(unicode[6..12]  | 0b10000000)
        // bytes[2] = u8(unicode[0..6]   | 0b10000000)
        bytes[0] = (uint8_t)(unicode >> 12) | 0b11100000;
        bytes[1] = (uint8_t)((unicode >> 6) & 0b111111) | 0b10000000;
        bytes[2] = (uint8_t)(unicode & 0b111111) | 0b10000000;
        return 3;
    } else if (unicode < 0x110000) { // unicode[0..21]
        // bytes[0] = u8(unicode[18..21] | 0b11110000)
        // bytes[1] = u8(unicode[12..18] | 0b11000000);
        // bytes[2] = u8(unicode[6 ..12] | 0b11000000);
        // bytes[3] = u8(unicode[0 ..6]  | 0b11000000);
        bytes[0] = (uint8_t)(unicode >> 18) & 0b111;
        bytes[1] = (uint8_t)((unicode >> 12) & 0b111111) | 0b10000000;
        bytes[2] = (uint8_t)((unicode >> 6) & 0b111111) | 0b1000000;
        bytes[3] = (uint8_t)(unicode);
        return 4;
    } else {
        // TODO: Error handling.
        return 0;
    }
}

struct char_slice_s utf8_from_bsf_static_string(struct bsf_static_string_s static_string) {
    uint32_t* unicode_chars = static_string.data;
    size_t howfar = static_string.len;
    struct char_buffer_s char_buffer = char_buffer_init();

    while (howfar) {
        uint32_t unicode = *(unicode_chars++);
        uint8_t bytes[4];

        size_t written_len = utf8_char_from_unicode(unicode, bytes);
        for (size_t byte_index = 0; byte_index < written_len; byte_index++) {
            char_buffer_push(&char_buffer, bytes[byte_index]);
        }

        howfar--;
    }

    return char_buffer_to_slice(char_buffer);
}
