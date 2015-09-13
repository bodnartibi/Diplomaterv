`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:24:32 09/08/2015
// Design Name:   Top
// Module Name:   C:/Users/Tibor/xilinx_projects/dipterv_modules/top_test.v
// Project Name:  dipterv_modules
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Top
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module top_test;

	integer i = 0;

	// Inputs
	reg [9:0] cntr;
	reg cntr_valid;
	reg rst;
	reg clk;
	reg data_out_read;

	// Outputs
	wire [31:0] data_out;
	wire data_out_valid;

	// Instantiate the Unit Under Test (UUT)
	Top uut (
		.cntr(cntr), 
		.cntr_valid(cntr_valid), 
		.rst(rst), 
		.clk(clk), 
		.data_out(data_out), 
		.data_out_valid(data_out_valid), 
		.data_out_read(data_out_read)
	);

	task value(input [9:0] cntr_);
	begin
		#2
		cntr = cntr_;
		cntr_valid = 1'b1;
		#2
		cntr_valid = 1'b0;
	end
	endtask

	initial begin
		// Initialize Inputs
		cntr = 0;
		cntr_valid = 0;
		rst = 0;
		clk = 1;
		data_out_read = 0;

		// Wait 100 ns for global reset to finish
		#10;
        
		// Add stimulus here
		rst = 1;
		#2
		rst = 0;
		#2
		value(10'd800);
		value(10'd200);
		value(10'd800);
		value(10'd200);
		value(10'd800);
		value(10'd200);
		value(10'd800);
		value(10'd200);
		#2
		data_out_read = 1;
		cntr = 10'd800;
		cntr_valid = 1'b1;
		#2
		cntr_valid = 1'b0;
		#2 #2 #2
		data_out_read = 0;
		#2
		while(i < 17)
		begin
			value(10'd200);
			value(10'd800);
			i = i + 1;
		end
		#2
		data_out_read = 1;
		#2 #2 #2
		data_out_read = 0;
		#6
		data_out_read = 1;
		

	end
	
	always #1 clk = ~clk;
      
endmodule

