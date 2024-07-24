#include "cli.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// <bin> <in_file_encoding> <out_file_encoding> (--input-file <file_name>|--stdin) (--output-file
// <file_name>|--stdout)

const char* ENCODING_WASNOT_RECOGNIZED_ERROR = "Encoding couldn't be recognized";
const char* UNKNOWN_ARGUMENT_ERROR = "Unknown argument";

const char* HELP = "\
Usage: dutfconv <in_encoding> <out_encoding> <input_source> <output_source>\n\
  in_encoding, out_encoding:\n\
    utf8                       UTF-8\n\
    utf16le                    UTF-16 Little Endian\n\
    utf16be                    UTF-16 Big Endian\n\
    utf32le                    UTF-32 Little Endian\n\
    utf32be                    UTF-32 Big Endian\n\
  input_source:\n\
    --stdin                    Get input from stdin\n\
    --input-file <file>        Get input from specified file\n\
  output_source:\n\
    --stdout                   Write output to stdout\n\
    --output-file <file>       Write output to special file\n\
";

struct args_iterator_s {
    const char** arg_ptr;
    size_t howfar;
};

static const struct args_iterator_s args_iterator_init(size_t count, const char** args) {
    struct args_iterator_s iterator = {.arg_ptr = args, .howfar = count};
    return iterator;
}

/* Returns 0 if next value was written, otherwise returns 1. */
static int args_iterator_next(struct args_iterator_s* self, const char** var) {
    if (!self->howfar) {
        return 1;
    }

    *var = *self->arg_ptr++;
    self->howfar--;

    return 0;
}

/* Returns 0 if there the function could parse string into encoding, otherwise returns 1. */
static int encoding_from_string(const char* string, enum encoding_e* encoding) {
    if (!strcmp(string, "utf8")) {
        *encoding = ENCODING_UTF8;
    } else if (!strcmp(string, "utf16le")) {
        *encoding = ENCODING_UTF16LE;
    } else if (!strcmp(string, "utf16be")) {
        *encoding = ENCODING_UTF16BE;
    } else if (!strcmp(string, "utf32le")) {
        *encoding = ENCODING_UTF32LE;
    } else if (!strcmp(string, "utf32be")) {
        *encoding = ENCODING_UTF32BE;
    } else {
        return 1;
    }

    return 0;
}

/* Assuming that given arguments (in second argument) will live longer than given `cli_args_s`. */
void parse_args(int count, const char** args, struct cli_args_s* cli_args) {
    struct args_iterator_s args_iterator = args_iterator_init(count - 1, args + 1);

    int input_enc_caught = 0, output_enc_caught = 0, input_caught = 0, output_caught = 0;

    const char* arg;
    while (!args_iterator_next(&args_iterator, &arg)) {
        if (!strcmp(arg, "help")) {
            printf("%s", HELP);
            exit(EXIT_SUCCESS);
        } else if (!strcmp(arg, "--stdin")) {
            cli_args->input_mode = INPUT_MODE_STDIN;

            input_caught = 1;
        } else if (!strcmp(arg, "--input-file")) {
            const char* input_file_name;
            if (args_iterator_next(&args_iterator, &input_file_name)) {
                struct error_s err = {.kind = ERROR_KIND_FATAL,
                                      .message = "Couldn't find <file_name> --input-file"};
                error_stack_push(err);
            }

            cli_args->input_mode = INPUT_MODE_FILE;
            cli_args->input_file_name = input_file_name;

            input_caught = 1;
        } else if (!strcmp(arg, "--stdout")) {
            cli_args->output_mode = OUTPUT_MODE_STDOUT;

            output_caught = 1;
        } else if (!strcmp(arg, "--output-file")) {
            const char* output_file_name;
            if (args_iterator_next(&args_iterator, &output_file_name)) {
                struct error_s err = {.kind = ERROR_KIND_FATAL,
                                      .message = "Couldn't find <file_name> --output-file"};
                error_stack_push(err);
            }

            cli_args->output_mode = OUTPUT_MODE_FILE;
            cli_args->output_file_name = output_file_name;

            output_caught = 1;
        } else if (input_enc_caught && !output_enc_caught) {
            // Input encoding was caught and output encoding wasn't, so `arg` is output encoding.
            enum encoding_e out_file_encoding;
            if (encoding_from_string(arg, &out_file_encoding)) {
                struct error_s err = {.kind = ERROR_KIND_FATAL,
                                      .message = ENCODING_WASNOT_RECOGNIZED_ERROR};
                error_stack_push(err);
            }

            cli_args->out_file_encoding = out_file_encoding;

            output_enc_caught = 1;
        } else if (!input_enc_caught && output_enc_caught) {
            // Output encoding was caught and input encoding wasn't, so `arg` is output encoding.
            enum encoding_e in_file_encoding;
            if (encoding_from_string(arg, &in_file_encoding)) {
                struct error_s err = {.kind = ERROR_KIND_FATAL,
                                      .message = ENCODING_WASNOT_RECOGNIZED_ERROR};
                error_stack_push(err);
            }

            cli_args->in_file_encoding = in_file_encoding;

            input_enc_caught = 1;
        } else if (!input_enc_caught && !output_enc_caught) {
            enum encoding_e in_file_encoding;
            if (encoding_from_string(arg, &in_file_encoding)) {
                struct error_s err = {.kind = ERROR_KIND_FATAL,
                                      .message = ENCODING_WASNOT_RECOGNIZED_ERROR};
                error_stack_push(err);
            }

            cli_args->in_file_encoding = in_file_encoding;

            input_enc_caught = 1;
        } else {
            struct error_s err = {.kind = ERROR_KIND_FATAL, .message = UNKNOWN_ARGUMENT_ERROR};
            error_stack_push(err);
        }
    }

    if (!input_enc_caught) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "You forgot to specify input encoding"};
        error_stack_push(err);
    }
    if (!output_enc_caught) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "You forgot to specify output encoding"};
        error_stack_push(err);
    }
    if (!input_caught) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "You forgot to specify input stream"};
        error_stack_push(err);
    }
    if (!output_caught) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "You forgot to specify output stream"};
        error_stack_push(err);
    }
}
