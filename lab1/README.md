# Lab 1: Barrel Shifter - Group 10
lab-1-barrel-shifter-group10 created by GitHub Classroom
<br></br>
### Hardware Used

Digilent's Nexys Artix-7 FPGA Board

### Software Used

Vivado 2019.2
<br></br>

[Link to Video Demo](https://youtu.be/Vt-jtBQ13us)
<br></br>
### Tables:

<table>
<tr>
<th>Cell Usage:</th>
<th>Instance Areas:</th>
</tr>
<tr>
<td>



|  **Part 1**  | **Cell** | **Count** |
| :----:  | :---  |:----: |
| 1 | LUT6 | 24 |
| 2   | IBUF | 12 |
| 3   | OBUF | 8 |
|  **Part 2** | **Cell** | **Count** |
| 1 | LUT6 | 24 |
| 2   | IBUF | 12 |
| 3   | OBUF | 8 |

  </td><td>

|  **Part 1**  | **Instance** | **Module** | **Cells** |
| :----:  | :---  |:--- | :----: |
| 1 | top |  | 44|
| 2   | mux2_unit | multi_barrel_shifter_mux | 24 |
|  **Part 2**  | **Instance** | **Module** | **Cells** |
| 1 | top |  | 44|
| 2   | reverser_unit1 | multi_barrel_shifter_reverser | 24 |
</td></tr></table>

<br></br>
## Code: 
```SystemVerilog
{
  module Barrel_shifter(a, amt, lr, y);
    input logic [7:0] a;
    input logic [2:0] amt;
    input logic lr;
    output logic [7:0] y;
    
   //);
   
   logic [7:0] left, right;
// testing with mux
 /*
   param_right_shifter #(.N(8)) unit1
   (.a(a), .amt(amt), .y1(right));
   
   param_left_shifter #(.N(8)) unit2
        (.a(a), .amt(amt), .y2(left));
   
   multi_barrel_shifter_mux  #(.N(8)) mux2_unit
        (.x0(left), .x1(right), .s(lr), .y(y));
*/        
        

// testing using the reverse

   param_right_shifter #(.N(8)) unit1
   (.a(right), .amt(amt), .y1(left));
           
   multi_barrel_shifter_reverser #(.N(8)) reverser_unit
        (.y(a), .lr(lr), .y_reversed(right));
        
   multi_barrel_shifter_reverser #(.N(8)) reverser_unit1
        (.y(left), .lr(lr), .y_reversed(y));
  
endmodule


module param_left_shifter
    #( parameter N=8 , parameter width=$clog2(N))
    (
    input logic [N-1:0] a,
    input logic [width-1:0] amt,
    output logic [N-1:0] y2
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
    assign y2 = s[width-1];
          
endmodule

module param_right_shifter
    #( parameter N=8 , parameter width= $clog2(N))
    (
    input logic [N-1:0] a,
    input logic [width-1:0] amt,
    output logic [N-1:0] y1
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
    assign y1 = s[width-1];
      
endmodule

module multi_barrel_shifter_mux
 #( parameter N=8 )
 (
    input logic [N-1:0] x0,
    input logic [N-1:0] x1,
    input logic s,
    output logic [N-1:0] y
    );

    
    always_comb
    //always@(s)
        if (~s)           
            y = x0;
        else
            y = x1;
            
 
  
endmodule

module multi_barrel_shifter_reverser
 #(parameter N=8)
 (
    input logic [N-1:0] y,
    input logic lr,
    output logic [N-1:0] y_reversed
    );
   logic [N-1:0] temp;
   
    generate 
        genvar i;
        for ( i =0 ; i<N ; i=i+1)
            assign temp[i] = y[N-i-1];
    endgenerate
   
    always_comb
        if (lr)
            y_reversed = temp;
        else
            y_reversed = y;
        
endmodule
}
``` 
