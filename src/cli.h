#include <stddef.h>

enum encoding_e {
    ENCODING_UTF8,
    ENCODING_UTF16LE,
    ENCODING_UTF16BE,
    ENCODING_UTF32LE,
    ENCODING_UTF32BE,
};

struct cli_args_s {
    enum encoding_e in_file_encoding;
    enum encoding_e out_file_encoding;

    enum {
        INPUT_MODE_STDIN,
        INPUT_MODE_FILE,
    } input_mode;
    const char* input_file_name; // Used only if `input_mode == INPUT_MODE_FILE`.

    enum {
        OUTPUT_MODE_STDOUT,
        OUTPUT_MODE_FILE,
    } output_mode;
    const char* output_file_name; // Used only if `output_mode == OUTPUT_MODE_FILE`.
};

/* Parses `cli_args_s` from given arguments and their count.
 *
 * The function can raise error if arguments are wrong using global error stack. */
void parse_args(int, const char**, struct cli_args_s*);
