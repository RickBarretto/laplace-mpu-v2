#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "laplace.h"
#include "parser.h"
#include "ui.h"

typedef struct  {
    Connection connection;
    bool connected;
} Bridge;

Bridge connect() {
    return (Bridge) {
        .connection = mpu_new_connection(),
        .connected = true
    };
}

void disconnect(Bridge *bridge) {
    mpu_close_connection(&bridge->connection);
    bridge->connected = false; 
}

#define with_connection(bridge)   \
    for (Bridge bridge = connect(); bridge.connected; disconnect(&bridge))


void execute(
    Instruction instruction, 
    PinIO pins, 
    Matrix matrix_a, 
    Matrix matrix_b, 
    Matrix result
) {
        instruction.base_cmd = mpu_build_base_cmd(instruction.opcode, instruction.matrix_size);

        mpu_next_stage(pins, instruction.base_cmd);
        mpu_store(matrix_a, pins, instruction.base_cmd);
        mpu_store(matrix_b, pins, instruction.base_cmd);
        mpu_next_stage(pins, instruction.base_cmd);
        mpu_load(result, pins, instruction.base_cmd);
}


int main(void)
{
    // 1) open + map

    with_connection(bridge) {
        PinIO pins = {
            .cmd  = bridge.connection.base + PIO_CMD_OFFSET,
            .stat = bridge.connection.base + PIO_STAT_OFFSET
        };

        bool running = true;
        while (running) {
            Matrix      matrix_a = {0}, matrix_b = {0}, result = {0};
            Instruction instruction = {};

            i8 operation = get_operation();
            instruction.opcode = operation;
            switch (operation)
            {
            // Matrix x Matrix => Matrix
            case Add:
            case Sub:
            case MatrixMult: {
                int a_size = count_rows("input/a.lp");
                int b_size = count_rows("input/a.lp");
                instruction.matrix_size = a_size;

                if (not are_valid_matrices_size(a_size, b_size))
                    continue;

                if (not parse_matrix(matrix_a, instruction.matrix_size, "input/a.lp").ok)
                    continue;
                
                if (not parse_matrix(matrix_b, instruction.matrix_size, "input/b.lp").ok)
                    continue;

                execute(instruction, pins, matrix_a, matrix_b, result);

                print_matrix("Matrix A", matrix_a, instruction.matrix_size);
                print_matrix("Matrix B", matrix_b, instruction.matrix_size);
                print_op(operation);
                print_matrix("Result", result, instruction.matrix_size);
                wait_for_enter();
                    
                break;
            }
       

            // Matrix x int => Matrix
            case ScalarMult: {
                instruction.matrix_size = count_rows("input/a.lp");

                if (not is_valid_matrix_size(instruction.matrix_size))
                    continue;

                if (not parse_matrix(matrix_a, instruction.matrix_size, "input/a.lp").ok)
                    continue;

                Scalar scalar = parse_scalar("input/scalar.lp");

                if (scalar.ok) {
                    matrix_b[0][0] = scalar.value;
                    execute(instruction, pins, matrix_a, matrix_b, result);

                    print_matrix("Matrix", matrix_a, instruction.matrix_size);
                    print_value("Scalar", scalar.value);
                    print_op(operation);
                    print_matrix("Result", result, instruction.matrix_size);
                    wait_for_enter();

                }           
                else continue;

                break;
            }
                

            // Matrix => Matrix
            case Opposite:
            case Transpose: {
                instruction.matrix_size = count_rows("input/a.lp");

                if (not is_valid_matrix_size(instruction.matrix_size))
                    continue;

                if (not parse_matrix(matrix_a, instruction.matrix_size, "input/a.lp").ok)
                    continue;

                execute(instruction, pins, matrix_a, matrix_b, result);

                print_matrix("Matrix", matrix_a, instruction.matrix_size);
                print_op(operation);
                print_matrix("Result", result, instruction.matrix_size);
                wait_for_enter();
                break;
            }

            // Matrix => int
            case Determinant: {
                instruction.matrix_size = count_rows("input/a.lp");

                if (not is_valid_determinant_size(instruction.matrix_size))
                    continue;

                if (not parse_matrix(matrix_a, instruction.matrix_size, "input/a.lp").ok)
                    continue;
        
                execute(instruction, pins, matrix_a, matrix_b, result);

                print_matrix("Matrix", matrix_a, instruction.matrix_size);
                print_op(operation);
                print_value("Result", result[0][0]);
                wait_for_enter();

                break;
            }

            // App Operations
            case Quit: {
                running = false;
                break;
            }

            // Invalid entry
            default: {
                puts("Invalid operation!!!");
                break;
            }
            }
        }
    }
}