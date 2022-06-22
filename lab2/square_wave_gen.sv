
module square_wave_gen
(
input logic [3:0] m, 
input logic [3:0] n, 
input logic clk,
input logic reset,
output logic square_wave
    );
    
    // signal declaration:
    logic [7:0] q;
    logic [11:0] r_next, r_reg;
    integer total_time = (m+n)*10;
    integer m_time = m*10;
    
    // register segment
    always_ff @(posedge clk)
    begin
        if(reset)
            r_reg<=0;
        else    
            r_reg<=r_next;       
    end
    
    //next-state logic segment:
    always_comb
    begin
        if(r_reg==total_time-1)
            r_next=0;
        else
            r_next=r_reg+1;
     end    
    
    //ouput logic segment:
    assign square_wave = (r_reg <= (m_time-1)) ? 1'b1: 1'b0;
 
endmodule
