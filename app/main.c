#include <stdio.h>
#include <stdlib.h>

#include "laplace.h"
#include "parser.h"
#include "ui.h"


int main(void)
{
    Connection conn;
    PinIO      pins;
    Instruction ins;
    Matrix      A, B, R;
    int         op, sz;

    // 1) open + map
    conn = mpu_new_connection();
    pins.cmd  = (volatile uint32_t *)
                   ((char*)conn.base + PIO_CMD_OFFSET);
    pins.stat = (volatile uint32_t *)
                   ((char*)conn.base + PIO_STAT_OFFSET);

    // 2) default data

    ins.matrix_size = count_rows("input/a.lp");
    parse_matrix(A, ins.matrix_size, "input/a.lp");
    parse_matrix(B, ins.matrix_size, "input/b.lp");

    // 3) user input
    op = get_operation();
    if (op < 0) return EXIT_FAILURE;
    ins.opcode = (unsigned)op;

    // 4) execute
    ins.base_cmd = mpu_build_base_cmd(ins.opcode, ins.matrix_size);

    mpu_next_stage(pins, ins.base_cmd);
    mpu_store(A, pins, ins.base_cmd);
    mpu_store(B, pins, ins.base_cmd);
    mpu_next_stage(pins, ins.base_cmd);
    mpu_load(R, pins, ins.base_cmd);

    // 5) print
    display_result(A, B, R, ins.opcode);

    // 6) cleanup
    mpu_close_connection(&conn);
    return EXIT_SUCCESS;
}