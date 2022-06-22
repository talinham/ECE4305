`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////


module square_pattern
   (
    input  logic [10:0] x, y,     // treated as x-/y-axis
    input  logic [15:0] sw, 
    output logic [11:0] back_rgb 
   );

   // declaration
   logic [3:0] up, down;
   logic [3:0] r, g, b;
   logic [7:0] size;
   
   always_comb
   begin
      case (sw[15:14]) 
         2'b00: begin
            size=16;
         end   
         2'b01: begin
            size=32;
         end 
         2'b10: begin
            size=64;
         end
         2'b11: begin
            size=128;
         end
         default: begin
            size=16;
         end  
      endcase
      // outside of square
      if (x <=(320 - size/2)|| x>=(320+size/2) || y<=(240-size/2) || y>=(240+size/2)) 
      begin
         r = ~sw[3:0];
         g = ~sw[7:4];
         b = ~sw[11:8];
      end   
      // square
      else 
      begin
         r = sw[3:0];
         g = sw[7:4];
         b = sw[11:8];
      end  

   end // always   
   // output
   assign back_rgb = {r, g, b};
endmodule

