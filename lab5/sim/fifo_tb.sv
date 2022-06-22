`timescale 1ns / 1ps

//////////////////////////////////////////////////////////////////////////////////


module fifo_tb();

    // signal declarations
    localparam DATA_WIDTH = 8;
    localparam ADDR_WIDTH = 3;                     
    localparam T = 10; //clock period
    
    logic clk, reset;
    logic wr, rd;
    logic [(2*DATA_WIDTH) -1: 0] w_data;        //2x data width of rd
    logic [DATA_WIDTH -1: 0]  r_data;               //added
    logic full, empty;
    
    // instantiate module under test
    fifo #(.DATA_WIDTH(DATA_WIDTH), .ADDR_WIDTH(ADDR_WIDTH)) uut (.*);
    
    // 10 ns clock running forever
    always
    begin
        clk = 1'b1;
        #(T / 2);
        clk = 1'b0;
        #(T / 2);
    end
    
    // reset for the first half cylce
    initial
    begin
        reset = 1'b1;
        rd = 1'b0;
        wr = 1'b0;
        @(negedge clk);
        reset = 1'b0;
    end
    
    //test vectors
    initial
    begin
        // ----------------EMPTY-----------------------
        // write
        @(negedge clk);
        w_data = 16'd1234;
        wr = 1'b1;     
        @(negedge clk);
        wr = 1'b0;
        
        // write
        repeat(1) @(negedge clk);
        w_data = 16'd5678;
        wr = 1'b1;
        @(negedge clk)
        wr = 1'b0;
        
        // write
        repeat(1) @(negedge clk);
        w_data = 16'd9123;
        wr = 1'b1;
        @(negedge clk)
        wr = 1'b0;        
        
//         //read
//        repeat(1) @(negedge clk);
//        rd = 1'b1;
//        @(negedge clk)
//        rd = 1'b0;
        
//         //read
//        repeat(1) @(negedge clk);
//        rd = 1'b1;
//        @(negedge clk)
//        rd = 1'b0;                
        
//        // write
//        repeat(1) @(negedge clk);
//        w_data = 16'd10875;
//        wr = 1'b1;
//        @(negedge clk)
//        wr = 1'b0;        
        
        // write
        repeat(1) @(negedge clk);
        w_data = 16'd25227;
        wr = 1'b1;
        @(negedge clk)
        wr = 1'b0;        
        
        // ----------------FULL-----------------------
        // read 1
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // read 2
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // read 3
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // read 4
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // read 5
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // read 6
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // read 7 
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // read 8
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;
        
        // ----------------EMPTY-----------------------
        
        // read & write at the same time
        repeat(1) @(negedge clk);
        w_data = 16'd7712;
        wr = 1'b1;
        rd = 1'b1;
        @(negedge clk)
        wr = 1'b0;
        rd = 1'b0;
        
        // read while empty
        repeat(1) @(negedge clk);
        rd = 1'b1;
        @(negedge clk)
        rd = 1'b0;

        // ----------------NOT EMPTY-----------------------
        repeat(1) @(negedge clk);
        w_data = 16'd62040;
        wr = 1'b1;
        @(negedge clk)
        wr = 1'b0;
        
        repeat(1) @(negedge clk);
        w_data = 16'd50578;
        wr = 1'b1;
        @(negedge clk)
        wr = 1'b0;
        
        repeat(1) @(negedge clk);
        w_data = 16'd6089;
        wr = 1'b1;
        @(negedge clk)
        wr = 1'b0;
        
        // read & write at the same time
        repeat(1) @(negedge clk);
        w_data = 16'd14612;
        wr = 1'b1;
        rd = 1'b1;
        @(negedge clk)
        wr = 1'b0;
        rd = 1'b0;
        
        repeat(5) @(negedge clk);
        $stop;
                
        
    end
endmodule

