`timescale 1ns / 1ps

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

		#10;
      rst = 1;
		#2
		rst = 0;
		#2
		data_in = 1;
		#20;

	end
	
	always #1 clk = ~clk;
      
endmodule

