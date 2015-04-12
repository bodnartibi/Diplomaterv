`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   20:12:31 04/12/2015
// Design Name:   Threshold
// Module Name:   C:/Users/Tibor/xilinx_projects/dipterv_modules/threshold_test.v
// Project Name:  dipterv_modules
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Threshold
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module threshold_test;

	// Inputs
	reg [9:0] cntr;
	reg cntr_valid;
	reg rst;
	reg clk;

	// Outputs
	wire detect;

	// Instantiate the Unit Under Test (UUT)
	Threshold uut (
		.cntr(cntr), 
		.cntr_valid(cntr_valid), 
		.rst(rst), 
		.clk(clk), 
		.detect(detect)
	);


	// egy ertek erkezik
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
		clk = 0;

		// Wait 100 ns for global reset to finish
		#10;
        
		// Add stimulus here

		rst = 1;
		#2
		
		rst = 0;
		
		value(10'd400);
		value(10'd600);
		value(10'd800);
		value(10'd900);
		value(10'd700);
		value(10'd800);
		value(10'd200);
		value(10'd100);
		value(10'd600);
		value(10'd700);
		value(10'd600);
		value(10'd800);
		value(10'd200);
	end
      
	always #1 clk = ~clk;
		
endmodule

