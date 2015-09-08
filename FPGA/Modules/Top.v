`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:08:12 09/08/2015 
// Design Name: 
// Module Name:    Top 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Top(
	input [9:0] cntr,
	input cntr_valid,
	input rst,
	input clk,
	output [31:0] data_out,
	output data_out_valid,
	input data_out_read
    );

wire detect_threshold2timer;
wire [31:0] timer_timer2buffer;
wire timer_valid_timer2buffer;
wire ack_buffer2timer;

Threshold threshold_1(
	.cntr(cntr),
   .cntr_valid(cntr_valid),
	.rst(rst),
	.clk(clk),
   .detect(detect_threshold2timer)
);

Timer timer_1(
    .clk(clk),
    .rst(rst),
    .detect(detect_threshold2timer),
    .timer_out(timer_timer2buffer),
	 .timer_valid(timer_valid_timer2buffer),
	 .ack(ack_buffer2timer)
    );
	 
Buffer buffer_1(
    .data_in(timer_timer2buffer),
    .data_in_valid(timer_valid_timer2buffer),
	 .data_in_ack(ack_buffer2timer),
	 .data_out(data_out),
	 .data_out_valid(data_out_valid),
	 .data_out_read(data_out_read),
    .clk(clk),
    .rst(rst)
    );

endmodule
