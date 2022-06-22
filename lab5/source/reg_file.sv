`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/25/2022 04:14:22 PM
// Design Name: 
// Module Name: reg_file
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module reg_file 
    #(parameter ADDR_WIDTH = 3, DATA_WIDTH = 8)
    (
        input logic clk,
        input logic w_en,
        input logic [ADDR_WIDTH - 1: 0] r_addr, // reading address
        input logic [ADDR_WIDTH : 0] w_addr, // writing address
        input logic [(2*DATA_WIDTH) - 1: 0] w_data,  // write port twice the width of read port
        output logic [DATA_WIDTH - 1: 0] r_data
    );
    
    // signal declaration
    logic [DATA_WIDTH - 1: 0] memory [0: 2 ** ADDR_WIDTH - 1];     // keeping the memory the same, increasing the pointer
    
    logic [DATA_WIDTH - 1: 0] high_byte;                              
    logic [DATA_WIDTH - 1: 0] low_byte;                              
    
    assign {high_byte, low_byte} = w_data; 
   
    // write operation
    always_ff @(posedge clk)
    begin
        if (w_en)
        begin
            memory[w_addr] <= low_byte;
            memory[w_addr+1] <= high_byte;
        end
    end
            
    // read operation
    assign r_data = memory[r_addr];
endmodule
