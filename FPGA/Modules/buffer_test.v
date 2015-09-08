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

	integer i = 0;

	// Inputs
	reg [7:0] data_in;
	reg data_in_valid;
	reg data_out_read;
	reg rst;
	reg clk;

	// Outputs
	wire [7:0] data_out;
	wire data_out_valid;
	wire data_in_ack;

	// Instantiate the Unit Under Test (UUT)
	Buffer uut (
		.data_in(data_in), 
		.data_in_valid(data_in_valid),
		.data_in_ack(data_in_ack),
		.data_out(data_out), 
		.data_out_valid(data_out_valid), 
		.data_out_read(data_out_read), 
		.rst(rst), 
		.clk(clk)
	);

	task data_input(input [7:0] data);
	begin
		data_in_valid = 1;
		data_in = data;
		wait(data_in_ack);
		#2
		data_in_valid = 0;
		#2;
	end
	endtask
	
	task read_all();
	begin
		data_out_read = 1;
		#2
		while(data_out_valid)
		begin
			#2;
		end
		data_out_read = 0;
	end
	endtask


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
		data_input(8'd1);
		data_input(8'd2);
		#2
		data_in_valid = 1;
		data_out_read = 1;
		data_in = 9'd3;
		wait(data_in_ack);
		data_out_read = 0;
		#2		
		data_input(8'd4);
		data_input(8'd5);
		data_input(8'd6);
		data_input(8'd7);
		read_all();
		
		while(i < 17)
		begin
			data_input(i);
			i = i + 1;
		end
	end
	
	always #1 clk = ~clk;
      
endmodule

