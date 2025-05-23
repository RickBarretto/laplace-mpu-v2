#ifndef MATRIX_PARSER_H
#define MATRIX_PARSER_H

#include <iso646.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "laplace.h"

#define MATRIX_MAX_SIZE 5

/// @struct Scalar result from parsing.
/// @public value : scalar result. 0 by default.
/// @public ok : the status of the parsing.
typedef struct Scalar {
    int8_t value;
    bool ok;
} Scalar;

/// @brief Check if string is a comment line
/// @param line content
/// @return bool
///
/// All comments starts with '#', just like Python or Bash.
bool _is_comment(cstring line) {
    return line[0] == '#';
}

bool _is_space(cstring line) {
    return (line[0] == ' ')
        or (line[0] == '\t')
        or (line[0] == '\r')
        or (line[0] == '\n')
        or (line[0] == '\0');
}

/// @brief Check if string is a matrix's row
/// @param line content
/// @return bool
///
/// A row is defined as: '[<num> <num> <num> ...]'.
/// You don't need comma, only open and close square braces.
bool _is_row(cstring line) {
    return line[0] == '[';
}

/// @brief Opens a file and prinrs the error,
/// @param filename 
/// @return FILE*. Otherwise, nil.
FILE* _open_file(cstring filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open input file: %s.\n", filename);
        return nil;
    }
    return file;
}

/// @brief Opens a file and closes at the end of the scope.
///
/// This macro apply a Python-like syntax for openning files, 
/// closing at the end automatically.
///
/// Usage
/// -----
///         with_open("file.txt", file) {
///             ... 
///         } // file is closed and set to NULL here.
#define with_open(filename, file)           \
    for (                                   \
        FILE *file = _open_file(filename);  \
        file != nil;                        \
        fclose(file), file = nil            \
    )

/// @brief Counts the size of a matrix.
/// @param filename 
/// @return matrix's size or -1 for error.
i8 count_rows(cstring filename) {
    i8 tolerance = MATRIX_MAX_SIZE + 1;
    i8 count = -1;

    with_open(filename, file) {
        char line[256];
        count = 0;
        while (fgets(line, sizeof(line), file) and count <= tolerance) {
            if (_is_row(line))
                count++;
        }
    }

    return (count <= MATRIX_MAX_SIZE)?
        count : -1;
}

/// @brief Writes the matrix froma file into `matrix`.
/// @param matrix Destiny matrix
/// @param matrix_size size of the matrix
/// @param filename file input
/// @return parsing's status.
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
        return err();
    }

    with_open(filename, file) {
        char line[256] = "";
        size_t line_count = 1;
        size_t row = 0;

        while (fgets(line, sizeof(line), file) and row < matrix_size) {
            int8_t a = 0, b = 0, c = 0, d = 0, e = 0;
            int8_t line_count = row + 1;

            if (_is_row(line)) {
                if (sscanf(line, format, &a, &b, &c, &d, &e) <= 0) {
                    printf("[line: %d] Invalid syntax.\n", line_count);
                    return err();
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

    return ok();
}

/// @brief Gets the scalar value from a file.
/// @param filename input file's path
/// @return scalar result
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

#endif