`timescale 1ns / 1ps

module threshold_test;

	// Inputs
	reg [31:0] data;
	reg data_valid;
	reg rst;
	reg clk;
	reg [31:0] HIGH;
	reg ack;
	reg [31:0] zero_num;

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
		.zero_num(zero_num), 
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
		data = 0;
		data_valid = 0;
		rst = 0;
		clk = 1;
		HIGH = 32'd150;
		ack = 0;
		zero_num = 32'd3;

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
		#2
		ack = 1;
		#2
		ack = 0;
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
		
	end
      
	always #1 clk = ~clk;
	
endmodule

