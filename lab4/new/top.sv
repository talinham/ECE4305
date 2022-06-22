`timescale 1ns / 1ps


module top
    (
        input logic clk,
        input logic reset,
        input logic [7:0] SW,
        input logic signal,
        output logic [6:0] sseg,
        output logic [7:0] an
        
    );
    
    localparam N=300;
    
    logic [7:0] tempConversion;
    logic [7:0] input_data;
    logic [11:0] in_temp, out_temp;
    
    always_comb
    begin
        if(~signal)
            if(SW>100)
                input_data =101;
            else
                input_data = SW;     
        else
            if(SW<32)
                input_data = 101;
            else
                input_data = SW+69;
    end
    
    
    rom_with_file #(.N(N)) run(.clk(clk), .data(input_data), .conv_data(tempConversion));
    
    
    binary2BCD in (
        .binary  (SW),
        .ones    (in_temp[3:0]),
        .tens    (in_temp[7:4]),
        .hundreds(in_temp[11:8])
      );
      
    binary2BCD out (
        .binary  (tempConversion),
        .ones    (out_temp[3:0]),
        .tens    (out_temp[7:4]),
        .hundreds(out_temp[11:8])
      );
    
    time_mux_display disp (
        .in0({1'b1 ,in_temp[3:0], 1'b1}),
        .in1({1'b1 ,in_temp[7:4], 1'b1}),
        .in2({1'b1 ,in_temp[11:8], 1'b1}),
        .in3(),
        .in4({1'b1 ,out_temp[3:0], 1'b1}),
        .in5({1'b1 ,out_temp[7:4], 1'b1}),
        .in6({1'b1 ,out_temp[11:8], 1'b1}),
        .in7(),
        .dp(),
        .*
    );
    
    
endmodule
