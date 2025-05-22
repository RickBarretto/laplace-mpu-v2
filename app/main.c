#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "laplace.h"
#include "parser.h"
#include "ui.h"


int main(void)
{
    // 1) open + map

    Connection connection = mpu_new_connection();
    PinIO pins = {
        .cmd  = connection.base + PIO_CMD_OFFSET,
        .stat = connection.base + PIO_STAT_OFFSET
    };

    // 2) default data

    Matrix      matrix_a, matrix_b, result = {0};
    Instruction instruction;

    instruction.matrix_size = count_rows("input/a.lp");
    if (not parse_matrix(matrix_a, instruction.matrix_size, "input/a.lp").ok)
        return EXIT_FAILURE;

    if (not parse_matrix(matrix_b, instruction.matrix_size, "input/b.lp").ok)
        return EXIT_FAILURE;

    // 3) user input
    instruction.opcode = get_operation();
    if (instruction.opcode < 0 or instruction.opcode > 6) return EXIT_FAILURE;

    if (instruction.opcode == 2) {
        Scalar scalar = parse_scalar("input/scalar.lp");
        if (!scalar.ok) {
            return EXIT_FAILURE;
        } else {
            matrix_b[0][0] = scalar.value;
        }
    }

    // 4) execute
    instruction.base_cmd = mpu_build_base_cmd(instruction.opcode, instruction.matrix_size);

    mpu_next_stage(pins, instruction.base_cmd);
    mpu_store(matrix_a, pins, instruction.base_cmd);
    mpu_store(matrix_b, pins, instruction.base_cmd);
    mpu_next_stage(pins, instruction.base_cmd);
    mpu_load(result, pins, instruction.base_cmd);

    // 5) print
    display_result(matrix_a, matrix_b, result, instruction.opcode);

    // 6) cleanup
    mpu_close_connection(&connection);
    return EXIT_SUCCESS;
}