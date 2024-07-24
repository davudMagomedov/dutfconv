#include <errno.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "basic_form.h"
#include "char_buffer.h"
#include "cli.h"
#include "error.h"

#define NONNULL_FD int

static inline NONNULL_FD open_input_file(const char* input_file_name) {
    NONNULL_FD file_fd = open(input_file_name, O_RDONLY);

    if (file_fd == -1) {
        errno_t sys_error = errno;
        struct error_s err;

        err.kind = ERROR_KIND_FATAL;

        switch (sys_error) {
        case ENOENT: {
            err.message = "Couldn't find input file";
            break;
        }
        case EACCES:
            err.message = "Input file doesn't have required permissions";
            break;
        default:
            err.message = "Coulnd't open file for unknown reason";
        }

        error_stack_push(err);
    }

    return file_fd;
}

static NONNULL_FD open_input(const struct cli_args_s* cli) {
    switch (cli->input_mode) {
    case INPUT_MODE_STDIN:
        return STDOUT_FILENO;
    case INPUT_MODE_FILE:
        return open_input_file(cli->input_file_name);
    }
}

static inline NONNULL_FD open_output_file(const char* output_file_name) {
    NONNULL_FD file_fd = open(output_file_name, O_RDWR | O_CREAT | O_APPEND);

    if (file_fd == -1) {
        errno_t sys_error = errno;
        struct error_s err = {.kind = ERROR_KIND_FATAL};

        switch (errno) {
        case EACCES:
            err.message = "Permissisons denied for output file";
            break;
        case ENOENT:
            err.message = "Couldn't find output file";
            break;
        default:
            err.message = "Coulnd't open output file for unknown reason";
        }

        error_stack_push(err);
    }

    return file_fd;
}

static NONNULL_FD open_output(const struct cli_args_s* cli) {
    switch (cli->output_mode) {
    case OUTPUT_MODE_STDOUT:
        return STDOUT_FILENO;
    case OUTPUT_MODE_FILE:
        return open_output_file(cli->output_file_name);
    }
}

static inline struct char_slice_s read_input_tty() {
    char buf;
    int code;

    struct char_buffer_s char_buffer = char_buffer_init();

    while ((code = read(STDIN_FILENO, &buf, 1)) > 0)
        char_buffer_push(&char_buffer, buf);

    if (code < 0) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "Couldn't read stdin because of unknown error."};
        error_stack_push(err);
    }

    return char_buffer_to_slice(char_buffer);
}

static inline struct char_slice_s read_input_file(NONNULL_FD file_fd) {
    struct stat st;
    if (fstat(file_fd, &st)) {
        struct error_s err = {.kind = ERROR_KIND_FATAL, .message = "An I/O error occured."};
        error_stack_push(err);
    }
    int file_size = st.st_size;

    char* buffer = malloc(st.st_size);
    if (read(file_fd, buffer, st.st_size) == -1) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "Couldn't read stdin because of unknown error."};
        error_stack_push(err);
    }

    struct char_slice_s char_slice = {.len = st.st_size, .data = buffer};

    return char_slice;
}

static void read_input(NONNULL_FD input_fd, struct char_slice_s* ss) {
    if (input_fd == STDIN_FILENO && isatty(input_fd)) {
        *ss = read_input_tty();
    } else {
        *ss = read_input_file(input_fd);
    }
}

static void write_output(NONNULL_FD output_fd, struct char_slice_s* ss) {
    int written = write(output_fd, ss->data, ss->len);

    if (written == -1) {
        struct error_s err = {.kind = ERROR_KIND_FATAL,
                              .message = "An I/O error occured during writing data fo file"};
        error_stack_push(err);
    }
}

int main(int argc, const char** argv) {
    struct cli_args_s cli_args;
    parse_args(argc, argv, &cli_args);

    NONNULL_FD input_fd = open_input(&cli_args);
    NONNULL_FD output_fd = open_output(&cli_args);

    struct char_slice_s input_data;
    read_input(input_fd, &input_data);

    struct bsf_static_string_s bsf_ss;
    struct char_iterator_s iterator = {.finger = input_data.data, .howfar = input_data.len};
    switch (cli_args.in_file_encoding) {
    case ENCODING_UTF8:
        bsf_ss = bsf_from_utf8(iterator);
        break;
    case ENCODING_UTF16LE:
        bsf_ss = bsf_from_utf16le(iterator);
        break;
    case ENCODING_UTF16BE:
        bsf_ss = bsf_from_utf16be(iterator);
        break;
    case ENCODING_UTF32LE:
        bsf_ss = bsf_from_utf32le(iterator);
        break;
    case ENCODING_UTF32BE:
        bsf_ss = bsf_from_utf32be(iterator);
        break;
    }

    struct char_slice_s output_data;
    switch (cli_args.out_file_encoding) {
    case ENCODING_UTF8:
        output_data = utf8_from_bsf_static_string(bsf_ss);
        break;
    case ENCODING_UTF16LE:
        output_data = utf16le_from_bsf_static_string(bsf_ss);
        break;
    case ENCODING_UTF16BE:
        output_data = utf16be_from_bsf_static_string(bsf_ss);
        break;
    case ENCODING_UTF32LE:
        output_data = utf32le_from_bsf_static_string(bsf_ss);
        break;
    case ENCODING_UTF32BE:
        output_data = utf32be_from_bsf_static_string(bsf_ss);
        break;
    }

    write_output(output_fd, &output_data);

    close(output_fd);
    close(input_fd);

    return 0;
}
