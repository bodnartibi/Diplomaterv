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
	reg clk_en;
	reg rst;
	reg new_data;
	reg din;
	reg [15:0] rate;
	reg rate_we;

	// Outputs
	wire [31:0] out;
	wire out_rdy;
	// Instantiate the Unit Under Test (UUT)
	CIC uut (
		.clk(clk), 
		.clk_en(clk_en), 
		.rst(rst), 
		.new_data(new_data), 
		.din(din), 
		.rate(rate), 
		.rate_we(rate_we)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		clk_en = 0;
		rst = 0;
		new_data = 0;
		din = 0;
		rate = 0;
		rate_we = 0;

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
      
endmodule

