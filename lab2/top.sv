`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////


module top(
        
        input logic [3:0]m,
        input logic [3:0]n,
        input logic clk, reset,
        output logic square_wave 
    );
    
    square_wave_gen M0 (.m(m), .n(n), .clk(clk), .reset(reset),  .square_wave(square_wave));
    
endmodule