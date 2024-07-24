#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "basic_form.h"
#include "char_iterator.h"
#include "error.h"

#undef NDEBUG

int compare_memory_byte(const uint8_t* m1, const uint8_t* m2, int n) {
    while (n--) {
        if (*(m1++) != *(m2++)) {
            return 0;
        }
    }

    return 1;
}

int compare_memory_int(const int* m1, const int* m2, int n) {
    while (n--) {
        if (*(m1++) != *(m2++)) {
            return 0;
        }
    }

    return 1;
}

void assert_string_utf8(const wchar_t* expect, const char* test_subject) {
    struct char_iterator_s ci = {.finger = test_subject, .howfar = strlen(test_subject)};
    struct bsf_static_string_s result = bsf_from_utf8(ci);

    printf("bsf_form_utf8(\"%s\"): ", test_subject);

    if (compare_memory_int(expect, result.data, result.len)) {
        printf("PASSED\n");
    } else {
        printf("BROKE\n");

        printf("  GOT: \t");
        for (size_t char_index = 0; char_index < result.len; char_index++) {
            printf("%X ", result.data[char_index]);
        }
        printf("\n");

        printf("  EXP: \t");
        for (size_t char_index = 0; char_index < result.len; char_index++) {
            printf("%X ", expect[char_index]);
        }
        printf("\n");
    }

    free(result.data);
}

void assert_string_utf16le(const wchar_t* expect, const char* test_subject, size_t len) {
    struct char_iterator_s ci = {.finger = test_subject, .howfar = len};
    struct bsf_static_string_s result = bsf_from_utf16le(ci);

    printf("bsf_form_utf16le(\"...\"): ", test_subject);

    if (compare_memory_int(expect, result.data, result.len)) {
        printf("PASSED\n");
    } else {
        printf("BROKE\n");

        printf("  GOT: \t");
        for (size_t char_index = 0; char_index < result.len; char_index++) {
            printf("%X ", result.data[char_index]);
        }
        printf("\n");

        printf("  EXP: \t");
        for (size_t char_index = 0; char_index < result.len; char_index++) {
            printf("%X ", expect[char_index]);
        }
        printf("\n");
    }

    free(result.data);
}

void assert_string_utf32le(const wchar_t* expect, const char* test_subject, size_t len) {
    struct char_iterator_s ci = {.finger = test_subject, .howfar = len};
    struct bsf_static_string_s result = bsf_from_utf32le(ci);

    printf("bsf_form_utf32le(\"...\"): ", test_subject);

    if (compare_memory_int(expect, result.data, result.len)) {
        printf("PASSED\n");
    } else {
        printf("BROKE\n");

        printf("  GOT: \t");
        for (size_t char_index = 0; char_index < result.len; char_index++) {
            printf("%X ", result.data[char_index]);
        }
        printf("\n");

        printf("  EXP: \t");
        for (size_t char_index = 0; char_index < result.len; char_index++) {
            printf("%X ", expect[char_index]);
        }
        printf("\n");
    }

    free(result.data);
}

void test_bsf_from_utf8() {
    assert_string_utf8(L"Hello world!", "Hello world!");
    assert_string_utf8(L"Привет мир!", "Привет мир!");
    assert_string_utf8(L"你好世界!", "你好世界!");
    assert_string_utf8(L"𝗛𝗘𝗟𝗟𝗢 𝗪𝗢𝗥𝗟𝗗!", "𝗛𝗘𝗟𝗟𝗢 𝗪𝗢𝗥𝗟𝗗!");
}

void test_bsf_from_utf16le() {
    const char test_bytes_1[] = {0x35, 0xd8, 0x3c, 0xde, 0x35, 0xd8, 0x3c, 0xde,
                                 0x42, 0x00, 0x42, 0x00, 0x14, 0x04, 0x14, 0x04};

    assert_string_utf16le(L"𝘼𝘼BBДД", test_bytes_1, sizeof(test_bytes_1));
}

void test_bsf_from_utf32le() {
    const wchar_t t[] = L"𝗛𝗘𝗟𝗟𝗢 𝗪𝗢𝗥𝗟𝗗!";

    assert_string_utf32le(t, t, wcslen(t));
}

void test_bsf_from_utf8le() {}

int main() {
    test_bsf_from_utf8();
    test_bsf_from_utf16le();
    test_bsf_from_utf32le();

    return 0;
}
