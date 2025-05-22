#ifndef MATRIX_PARSER_H
#define MATRIX_PARSER_H

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

int is_comment(char* line) {
    return line[0] == '#';
}

int is_space(char* line) {
    return (line[0] == ' ') 
        || (line[0] == '\t') 
        || (line[0] == '\r') 
        || (line[0] == '\n') 
        || (line[0] == '\0');
}

int is_row(char * line) {
    return line[0] == '[';
}

FILE* open_file(char *filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open input file.\n");
        return 0;
    }
    return file;
}

#define with_open(filename, fp, block)  \
    do {                                \
        FILE *fp = open_file(filename); \
        if (fp) {                       \
            block                       \
            fclose(fp);                 \
        }                               \
    } while (0)

int count_rows(char *filename) {
    char line[256];
    int count = 0;

    FILE *file = open_file(filename);

    with_open(filename, file, {
        if (!file) {
            printf("Failed to open file.\n");
            return -1;
        }

        while (fgets(line, sizeof(line), file)) {
            if (is_row(line))
                count++;
        }
    });

    return count;
}


int parse_matrix(Matrix matrix, uint8_t matrix_size, char* filename) {
    int i = 0;
    int line_count = 0;
    int a, b, c, d, e;
    char* format = "";
    char line[256];

    FILE *file = open_file(filename);

    switch (matrix_size)
    {
    case 1: format = "[%hhd]";                        break;
    case 2: format = "[%hhd %hhd]";                   break;
    case 3: format = "[%hhd %hhd %hhd]";              break;
    case 4: format = "[%hhd %hhd %hhd %hhd]";         break;
    case 5: format = "[%hhd %hhd %hhd %hhd %hhd]";    break;
    default:
        printf("Invalid matrix size\n");
        return -1;
    }

    with_open(filename, file, {
        while (fgets(line, sizeof(line), file)) {
            line_count = i + 1;

            if (is_row(line)) {
                a = 0; b = 0; c = 0; d = 0; e = 0;
                if (sscanf(line, format, &a, &b, &c, &d, &e) <= 0) {
                    printf("[line: %d] Invalid syntax.\n", line_count);
                    return -1;
                }

                matrix[i][0] = a;
                matrix[i][1] = b;
                matrix[i][2] = c;
                matrix[i][3] = d;
                matrix[i][4] = e;
                i++;
            }
        }
    });

    return 0;
}


Scalar parse_scalar(char* filename) {
    FILE *file = open_file(filename);
    int line_count = 1;
    char line[256] = "";

    Scalar result = {
        .value = 0,
        .ok = true,
    };

    with_open(filename, file,
    {
        while (fgets(line, sizeof(line), file))
        {
            if (is_comment(line) || is_space(line))
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
    });

    return result;
}