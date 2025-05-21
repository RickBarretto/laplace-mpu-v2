`define i8(x) 8'sd``x                /// defined a 8-bit integer
`define MATRIX_5x5 (0):(8*25-1)     /// defines a 5x5 matrix flatted array indexes
`define at(col, row) (8 * (row + 5*col))    /// Access each 8-bit element in the 5x5 matrix

module MpuSub (
    input      signed [8*25-1:0] matrix_a,  // Flattened 5x5 matrix of 8-bit elements
    input      signed [8*25-1:0] matrix_b,  
    output reg signed [8*25-1:0] result    
);

    genvar col, row;
    generate
        for (col = 0; col < 5; col = col + 1) begin : col_loop
            for (row = 0; row < 5; row = row + 1) begin : row_loop
                always @(*) begin
                    result[`at(col, row) +: 8] = 
                        matrix_a[`at(col, row) +: 8] - matrix_b[`at(col, row) +: 8];
                end
            end
        end
    endgenerate

endmodule
