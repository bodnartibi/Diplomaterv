`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   18:51:11 11/15/2015
// Design Name:   Threshold
// Module Name:   C:/Users/Tibor/xilinx_projects/CIC_filter/threshold_test.v
// Project Name:  CIC_filter
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
	reg [31:0] data;
	reg data_valid;
	reg rst;
	reg clk;
	reg [31:0] HIGH;
	reg [31:0] LOW;
	reg ack;

	// Outputs
	wire valid;
	wire [31:0] detect_time;

	// Instantiate the Unit Under Test (UUT)
	Threshold uut (
		.data(data), 
		.data_valid(data_valid), 
		.rst(rst), 
		.clk(clk), 
		.HIGH(HIGH), 
		.LOW(LOW), 
		.ack(ack), 
		.valid(valid), 
		.detect_time(detect_time)
	);

	task value(input [31:0] cntr_);
	begin
		#2
		data = cntr_;
		data_valid = 1'b1;
		#2
		data_valid = 1'b0;
		#4;
	end
	endtask

	initial begin
		// Initialize Inputs
		data = 0;
		data_valid = 0;
		rst = 0;
		clk = 1;
		HIGH = 32'd150;
		LOW = 0;
		ack = 0;

		// Wait 100 ns for global reset to finish
		#10;
		rst = 1;
		#2
		rst = 0;
		#2
		value(32'd0);
		value(32'd110);
		value(32'd160);
		value(32'd155);
		value(32'd170);
		value(32'd160);
		value(32'd0);
		value(32'd0);
		value(32'd0);
		value(32'd0);
		value(32'd0);
		value(32'd160);
		value(32'd170);
		value(32'd180);
		value(32'd190);
		value(32'd1170);
		value(32'd0);
		value(32'd0);
		value(32'd0);
		value(32'd0);
		value(32'd0);
		
        
		// Add stimulus here

	end
      
	always #1 clk = ~clk;
	
endmodule

