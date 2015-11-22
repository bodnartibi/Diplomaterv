`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   18:04:55 11/15/2015
// Design Name:   CIC
// Module Name:   C:/Users/Tibor/xilinx_projects/zynq_full/pcores/microphone_pre_v1_00_a/devl/projnav/CIC_test.v
// Project Name:  microphone_pre
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
	reg [5:0] comb_num;
	reg [7:0] dec_num;
	reg data_in;
	reg [31:0] clk_div;

	// Outputs
	wire [31:0] data_out;
	wire data_out_valid;
	wire channel;
	wire clk_out;

	// Instantiate the Unit Under Test (UUT)
	CIC uut (
		.clk(clk), 
		.rst(rst), 
		.clk_div(clk_div),
		.comb_num(comb_num), 
		.dec_num(dec_num), 
		.data_out(data_out), 
		.data_out_valid(data_out_valid), 
		.channel(channel), 
		.clk_out(clk_out), 
		.data_in(data_in)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		rst = 0;
		comb_num = 3;
		dec_num = 1;
		data_in = 0;
		clk_div = 2;

		// Wait 100 ns for global reset to finish
		#10;
      rst = 1;
		#2
		rst = 0;
		#2
		data_in = 1;
		#20;
		// Add stimulus here

	end
	
	always #1 clk = ~clk;
      
endmodule

