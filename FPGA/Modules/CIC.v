`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:46:18 07/06/2015 
// Design Name: 
// Module Name:    CIC 
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
module CIC(
	input clk,
	input clk_en,
	input rst,
	input new_data,
	input din,
	input [15:0] rate,
	input rate_we,
	output reg [31:0] out,
	output reg out_rdy
    );

reg [31:0] integ;

reg [15:0] dec_cntr;
reg [15:0] dec_num;

// N = 2
reg [31:0] comb1;
reg [31:0] comb2;

always@(posedge clk)
begin

if(rst)
begin
	integ <= 32'd0;
	dec_cntr <= 15'd0;
	dec_num <= 15'd0;
	comb1 <= 31'd0;
	comb2 <= 31'd0;
	out_rdy <= 1'b0;
end
else
begin
	
	//integrator
	integ <= integ + din;
	//decimator
	if(rate_we)
	begin
		dec_num <= rate;
	end
	dec_cntr <= dec_cntr + 1;
	if(dec_cntr == dec_num)
	begin
		comb1 <= integ;
		comb2 <= comb1;
		out <= integ - comb2;
		out_rdy <= 1'b1;
		dec_cntr <= 15'd0;
	end
	else
	begin
		out_rdy <= 1'b0;
	end
end

end // always
endmodule
