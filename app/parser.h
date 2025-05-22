#ifndef MATRIX_PARSER_H
#define MATRIX_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "laplace.h"

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
            if (line[0] == '[')
                count++;
        }
    });

    return count;
}


int parse_matrix(Matrix matrix, uint8_t matrix_size, char* filename) {
    int i = 0;
    int a, b, c, d, e;
    char* format = "";
    char line[256];

    FILE *file = open_file(filename);

    switch (matrix_size)
    {
    case 1: format = "[%d]";                break;
    case 2: format = "[%d %d]";             break;
    case 3: format = "[%d %d %d]";          break;
    case 4: format = "[%d %d %d %d]";       break;
    case 5: format = "[%d %d %d %d %d]";    break;
    default:
        printf("Invalid matrix size\n");
        return -1;
    }

    with_open(filename, file, {
        for (i = 0; i < matrix_size; i++) {
            if (!fgets(line, sizeof(line), file)) {
                printf("File's line doesn't match the matrix size\n");
                return -1;
            }

            if (line[0] == '#')
                continue;

            a = 0; b = 0; c = 0; d = 0; e = 0;
            if (sscanf(line, format, &a, &b, &c, &d, &e) <= 0) {
                printf("[line: %d] Invalid syntax.\n", i + 1);
                return -1;
            }

            matrix[i][0] = a;
            matrix[i][1] = b;
            matrix[i][2] = c;
            matrix[i][3] = d;
            matrix[i][4] = e;
        }
    });

    return 0;
}


int parse_scalar(char* filename) {
    FILE *file = open_file(filename);
    int line_count = 1;
    char line[256] = "";
    int value = 0;

    with_open(filename, file, {
        while (fgets(line, sizeof(line), file)) {
            if (is_comment(line) || is_space(line))
                continue;

            if (sscanf(line, "scalar: %d", &value) <= 0) {
                printf("[line %d] Invalid syntax.\n", line_count);
                puts("Example of valid syntax:");
                puts("scalar: 15");
                return -1;
            }
            line_count++;
        }
    });

    return 0;
}