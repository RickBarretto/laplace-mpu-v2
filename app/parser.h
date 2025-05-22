#ifndef MATRIX_PARSER_H
#define MATRIX_PARSER_H

#include <iso646.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "laplace.h"

typedef struct Scalar {
    int8_t value;
    bool ok;
} Scalar;

typedef struct Status {
    bool ok;
} Status;

int _is_comment(char* line) {
    return line[0] == '#';
}

int _is_space(char* line) {
    return (line[0] == ' ')
        or (line[0] == '\t')
        or (line[0] == '\r')
        or (line[0] == '\n')
        or (line[0] == '\0');
}

int _is_row(char * line) {
    return line[0] == '[';
}

FILE* _open_file(char *filename) {
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

int count_rows(char *filename) {
    char line[256];
    int count = 0;

    with_open(filename, file) {
        while (fgets(line, sizeof(line), file)) {
            if (_is_row(line))
                count++;
        }
    }

    return count;
}


Status parse_matrix(Matrix matrix, uint8_t matrix_size, char* filename) {
    int i = 0;
    char* format = "";
    char line[256];
    int8_t a, b, c, d, e = 0;

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
        while (fgets(line, sizeof(line), file)) {
            int8_t line_count = i + 1;

            if (_is_row(line)) {
                a, b, c, d, e = 0;
                if (sscanf(line, format, &a, &b, &c, &d, &e) <= 0) {
                    printf("[line: %d] Invalid syntax.\n", line_count);
                    return (Status){ .ok = false };
                }

                matrix[i][0] = a;
                matrix[i][1] = b;
                matrix[i][2] = c;
                matrix[i][3] = d;
                matrix[i][4] = e;
                i++;
            }
        }
    }

    return (Status){ .ok = true };
}


Scalar parse_scalar(char* filename) {
    int line_count = 1;
    char line[256] = "";

    Scalar result = {
        .value = 0,
        .ok = true,
    };

    with_open(filename, file) {
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
            }
        }
    }

    return result;
}