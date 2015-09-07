`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   21:19:30 04/12/2015
// Design Name:   Timer
// Module Name:   C:/Users/Tibor/xilinx_projects/dipterv_modules/timer_test.v
// Project Name:  dipterv_modules
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Timer
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module timer_test;

	// Inputs
	reg clk;
	reg rst;
	reg detect;
	reg ack;

	// Instantiate the Unit Under Test (UUT)
	Timer uut (
		.clk(clk), 
		.rst(rst), 
		.detect(detect),
		.ack(ack)
	);
	

	task detected(input [7:0] clk_per);
	begin
		#2
		detect = 1'b1;
		while(clk_per > 0)
			begin
				#2
				clk_per = clk_per - 1;
			end
		detect = 1'b0;
	end
	endtask

	initial begin
		// Initialize Inputs
		clk = 0;
		rst = 0;
		detect = 0;
		ack = 0;

		// Wait 100 ns for global reset to finish
		#10;
        
		// Add stimulus here
		
		rst = 1;
		#2
		rst = 0;
		
		detected(8'd1);
		detected(8'd2);
		#2
		ack = 1'b0;
		#2
		detected(8'd10);

	end
      
	always #1 clk = ~clk;
	
endmodule

