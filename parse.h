#pragma once

#include "common.h"

bool parse_piece(char **line_ptr, char *buffer, size_t buffer_size,
                 char separator, bool newline_is_separator);

int string_to_int(const char *string, int *result);

char *trim_whitespace(char *string);

int isBlank (const char * line);