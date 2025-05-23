#ifndef UI_H
#define UI_H

#include <stdio.h>

#include "laplace.h"


// ─── Library API ──────────────────────────────────────────────────────────


enum Operations {
    Add = 0,
    Sub = 1,
    ScalarMult = 2,
    Opposite = 3,
    Transpose = 4,
    Determinant = 5,
    MatrixMult = 6,
    Quit = 7,
    InvalidOperation = 8
};

const cstring op_repr[9] = {
    [Add] = "Add",
    [Sub] = "Sub",
    [ScalarMult] = "Scalar multiplication",
    [Opposite] = "Opposite",
    [Transpose] = "Transpose",
    [Determinant] = "Determinant",
    [MatrixMult] = "Matrix multiplication",
    [Quit] = "Quit",
    [InvalidOperation] = "Invalid operation"
};

/// @brief Read unsiged 8-bit integer from input
/// @param msg Prompt
/// @return User's input or -1 for error.
static int get_u8(const char *msg);

/// @brief Read operation code from User's input
/// @return operation's code or -1 for error.
///
/// Operations Table
/// ----------------
/// 
/// Code      Operation
/// ----  ------------------
///  0           Add
///  1           Sub
///  2    Multiply by Scalar 
///  3         Opposite
///  4        Transpose
///  5        Determinant
///  6     Multipliy Matrix
/// -1    Invalid Operation
static int get_operation(void);

/// @brief Wait for user input to continue
void wait_for_enter();

/// @brief Print a value on terminal
/// @param title Title of the section
/// @param value Value itself
void print_value(char* title, int value);


/// @brief Prints a 5x5 matrix on terminal
/// @param title Title of the section
/// @param M     Matrix itself
static void print_matrix(
    const char *title, 
    Matrix M,
    int size
);


// ─── Library Functions ────────────────────────────────────────────────────


// ─── Helpers ──────────────────────────────────────────────────────────────


int get_u8(const char *msg)
{
    int v;
    printf("%s\n>>> ", msg);
    if (scanf("%d", &v) != 1 || v < 0 || v > 255) {
        fprintf(stderr, "Entre 0 e 255.\n");
        return -1;
    }
    return v;
}


// ─── Library Functions ────────────────────────────────────────────────────


int get_operation(void)
{

    puts("Choose an operation...\n");

    for (int i = 0; i < 8; i++) {
        printf("%d. ", i);
        puts(op_repr[i]);
    }

    int op = get_u8("Digite o código da operação (0..7):");
    if (op < 0 || op > 7) {
        fprintf(stderr, "Operação inválida.\n");
        return -1;
    }
    return op;
}


void wait_for_enter() {
    puts("Press [Enter] to continue...");
    prinit(">>> ");
    while (getchar() != '\n');
    getchar();
}


void print_matrix(const char *title, Matrix M, int size)
{
    int r, c;
    printf("\n%s\n", title);
    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            printf("%4u", M[r][c]);
        }
        putchar('\n');
    }
}


void print_value(char* title, int value) {
    printf("\n%s\n", title);
    printf("%d\n", value);
}


void print_op(int op) {
    printf("\nOperation: %s\n", op_repr[op]);
}


#endif