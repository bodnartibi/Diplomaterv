`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   00:46:13 07/08/2015
// Design Name:   CIC
// Module Name:   C:/Users/Tibor/xilinx_projects/CIC_filter/CIC_imp_test.v
// Project Name:  CIC_filter
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: CIC
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module CIC_imp_test;

	// Inputs
	reg clk;
	reg rst;
	reg din;
	reg [15:0] rate;
	reg rate_we;
	reg [7:0] comb_num;

	// Outputs
	wire [31:0] out;
	wire out_rdy;
	// Instantiate the Unit Under Test (UUT)
	CIC uut (
		.clk(clk), 
		.rst(rst), 
		.din(din),
		.comb_num(comb_num),
		.dec_num(rate), 
		.out(out),
		.out_rdy(out_rdy)
	);
	
	initial begin
		// Initialize Inputs
		clk = 0;
		rst = 0;
		din = 0;
		rate = 15'd3;
		//rate = 0;
		comb_num = 8'd4;

		#2;
		
		rst = 1;
		#2;
		
		rst = 0;
		#2;
		
		#2;
		din = 1;
		#2;
		
		//din = 0;
		#2;
		
		#2;

	end
	
	always #1 clk = ~clk;
	//always #10 din = ~din;
      
endmodule

