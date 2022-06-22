`timescale 1ns / 1ps

module rom_with_file
    #(parameter N=500)
    (
        input logic clk,
        input logic [7:0] data, 
        output logic [7:0] conv_data
    );
    
    (*rom_style = "block" *)logic [7:0] rom [0:N]; 
    
    initial
        $readmemh("truth_table.mem", rom);
    
    
    always_ff @ (posedge clk)
        conv_data <= rom[data];
    
endmodule
