`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////

module param_left_shifter
    #( parameter N=8 , parameter width=$clog2(N))
    (
    input logic [N-1:0] a,
    input logic [width-1:0] amt,
    output logic [N-1:0] y
    );
    localparam stage = width;
    logic [N-1:0] s [width-1:0];
    
    assign s[0][N-1:0] = amt[0] ? {a[N-2:0], a[N-1]} : a;
    
    generate 
        genvar i;
        for ( i =1 ; i<stage ; i=i+1)
            assign s[i][N-1:0] = amt[i] ? {s[i-1][N-(2**i)-1:0],
                   s[i-1][(N-1): N-(2**i)]} : s[i-1];
    endgenerate
    assign y = s[width-1];
          
endmodule

module param_right_shifter
    #( parameter N=8 , parameter width= $clog2(N))
    (
    input logic [N-1:0] a,
    input logic [width-1:0] amt,
    output logic [N-1:0] y
    );
    localparam stage = width;
    logic [N-1:0] s [width-1:0];
    
    assign s[0][N-1:0] = amt[0] ? {a[0], a[N-1:1]} : a;
    
    generate 
        genvar i;
        for ( i =1 ; i<stage ; i=i+1)
            assign s[i][N-1:0] = amt[i] ? {s[i-1][(2**i)-1:0],
                   s[i-1][(N-1):(2**i)]} : s[i-1];
    endgenerate
    assign y = s[width-1];
      
endmodule


 
module barrel_insta
(
    input logic [7:0] a,
    input logic [2:0] amt,
    input logic lr,
    output logic [7:0] y_right, 
    output logic [7:0] y_left
    
   );
   
   param_right_shifter #(.N(8)) unit1
   (.a(a), .amt(amt), .y(y_right));
   
   param_left_shifter #(.N(8)) unit2
        (.a(a), .amt(amt), .y(y_left));
   
   
endmodule

module barrel_shifter_tb;
    logic [7:0] a;
    logic [2:0] amt;
    logic [7:0] y_right;
    logic [7:0] y_left;

    barrel_insta uut (.a(a), .amt(amt), .lr(lr), .y_right(y_right), .y_left(y_left));
    
    initial // initial block executes only once
        begin
            
            a = 'b11010010;
            amt = 'b001;
            # 130;
             
            amt = 'b010;
            # 130;
                       
            amt = 'b011;
            # 130;
                        
            amt = 'b100;
            # 130;
            
            amt = 'b101;
            # 130;
                       
            amt = 'b110;
            # 130;
                      
            amt = 'b111;
            # 130;
          
        end
endmodule

