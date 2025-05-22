#ifndef MATRIX_PARSER_H
#define MATRIX_PARSER_H

#include <iso646.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "laplace.h"

typedef struct Scalar {
    int8_t value;
    bool ok;
} Scalar;

int _is_comment(cstring line) {
    return line[0] == '#';
}

int _is_space(cstring line) {
    return (line[0] == ' ')
        or (line[0] == '\t')
        or (line[0] == '\r')
        or (line[0] == '\n')
        or (line[0] == '\0');
}

int _is_row(cstring line) {
    return line[0] == '[';
}

FILE* _open_file(cstring filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open input file: %s.\n", filename);
        return 0;
    }
    return file;
}

#define with_open(filename, file)           \
    for (                                   \
        FILE *file = _open_file(filename);   \
        file != NULL;                       \
        fclose(file), file = NULL           \
    )

int count_rows(cstring filename) {
    int count = 0;

    with_open(filename, file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            if (_is_row(line))
                count++;
        }
    }

    return count;
}


Status parse_matrix(Matrix matrix, uint8_t matrix_size, cstring filename) {
    cstring format = "";

    switch (matrix_size)
    {
    case 1: format = "[%hhd]";                        break;
    case 2: format = "[%hhd %hhd]";                   break;
    case 3: format = "[%hhd %hhd %hhd]";              break;
    case 4: format = "[%hhd %hhd %hhd %hhd]";         break;
    case 5: format = "[%hhd %hhd %hhd %hhd %hhd]";    break;
    default:
        printf("Invalid matrix size\n");
        return (Status){ .ok = false };
    }

    with_open(filename, file) {
        char line[256] = "";
        size_t line_count = 1;
        size_t row = 0;

        while (fgets(line, sizeof(line), file)) {
            int8_t a, b, c, d, e = 0;
            int8_t line_count = row + 1;

            if (_is_row(line)) {
                if (sscanf(line, format, &a, &b, &c, &d, &e) <= 0) {
                    printf("[line: %d] Invalid syntax.\n", line_count);
                    return (Status){ .ok = false };
                }

                matrix[row][0] = a;
                matrix[row][1] = b;
                matrix[row][2] = c;
                matrix[row][3] = d;
                matrix[row][4] = e;
                
                row++;
            }

            line_count++;
        }
    }

    return (Status){ .ok = true };
}


Scalar parse_scalar(cstring filename) {

    Scalar result = {
        .value = 0,
        .ok = true,
    };

    with_open(filename, file) {
        char line[256] = "";
        size_t line_count = 1;

        while (fgets(line, sizeof(line), file))
        {
            if (_is_comment(line) or _is_space(line))
                line_count++;
            else if (sscanf(line, "scalar: %hhd", &result.value) <= 0) {
                printf("[line %d] Invalid syntax.\n", line_count);
                puts("");
                puts("Example of valid syntax:");
                puts("scalar: 15");
                puts("");
                result.ok = false;
                return result;
            } else {
                return result;
            }
        }
    }
}