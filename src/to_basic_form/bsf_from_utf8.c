#include <stddef.h>
#include <stdint.h>

#include "../bsf_buffer.h"
#include "../char_iterator.h"

struct bsf_static_string_s bsf_from_utf8(struct char_iterator_s iterator) {
    struct bsf_buffer_s buffer = bsf_buffer_init();

    while (iterator.howfar) {
        uint8_t byte = char_iterator_next(&iterator);

        uint32_t unicode;
        if ((byte >> 7) == 0) {
            // byte[7] == 0 means that unicode code is byte[0..7].
            unicode = (uint32_t)byte;
        } else if ((byte >> 5) == 0b110) {
            // byte[5..8] == 0b110 means that unicode is (byte[0..5] << 6) + next_byte[0..6].
            const uint8_t next_byte = char_iterator_next(&iterator);
            unicode = ((uint32_t)(byte & 0b00011111) << 6) + ((uint32_t)next_byte & 0b00111111);
        } else if ((byte >> 4) == 0b1110) {
            // byte[4..8] == 0b1110 means that unicode is (byte[0..4] << 12) + (next_byte_1[0..6] <<
            // 6) + next_byte_2.
            const uint8_t next_byte_1 = char_iterator_next(&iterator);
            const uint8_t next_byte_2 = char_iterator_next(&iterator);
            unicode = ((uint32_t)(byte & 0b00001111) << 12) +
                      ((uint32_t)(next_byte_1 & 0b00111111) << 6) +
                      (uint32_t)(next_byte_2 & 0b00111111);
        } else if ((byte >> 3) == 0b11110) {
            // byte[3..8] == 0b11110 means unicode is (byte[0..3] << 18) + (next_byte_1[0..6] << 12)
            // + (next_byte_2[0..6] << 6) + next_byte_3[0..6].
            const uint8_t next_byte_1 = char_iterator_next(&iterator);
            const uint8_t next_byte_2 = char_iterator_next(&iterator);
            const uint8_t next_byte_3 = char_iterator_next(&iterator);
            unicode = ((uint32_t)(byte & 0b00000111) << 18) +
                      ((uint32_t)(next_byte_1 & 0b00111111) << 12) +
                      ((uint32_t)(next_byte_2 & 0b00111111) << 6) +
                      ((uint32_t)(next_byte_3 & 0b00111111));
        }

        bsf_buffer_push(&buffer, unicode);
    }

    return bsf_buffer_to_static_string(buffer);
}
