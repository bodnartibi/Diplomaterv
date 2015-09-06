`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   17:30:54 09/06/2015
// Design Name:   Buffer
// Module Name:   C:/Users/Tibor/xilinx_projects/dipterv_modules/buffer_test.v
// Project Name:  dipterv_modules
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Buffer
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module buffer_test;

	// Inputs
	reg [8:0] data_in;
	reg data_in_valid;
	reg data_out_read;
	reg rst;
	reg clk;

	// Outputs
	wire [8:0] data_out;
	wire data_out_valid;
	wire data_out_read_ack;
	wire data_in_ack;

	// Instantiate the Unit Under Test (UUT)
	Buffer uut (
		.data_in(data_in), 
		.data_in_valid(data_in_valid),
		.data_in_ack(data_in_ack),
		.data_out(data_out), 
		.data_out_valid(data_out_valid), 
		.data_out_read(data_out_read), 
		.data_out_read_ack(data_out_read_ack), 
		.rst(rst), 
		.clk(clk)
	);

	initial begin
		// Initialize Inputs
		data_in = 0;
		data_in_valid = 0;
		data_out_read = 0;
		rst = 0;
		clk = 0;

		// Wait 100 ns for global reset to finish
		#6;
		// Add stimulus here
		
		#2 rst = 1;
		#2 rst = 0;
		#2
		
		data_in_valid = 1;
		data_in = 9'b111111111;
		
		#2
		data_in = 9'b000001111;
		#2
		data_in_valid = 0;
		
		
		
		
		

	end
	
	always #1 clk = ~clk;
      
endmodule

