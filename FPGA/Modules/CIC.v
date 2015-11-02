`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:11:36 10/24/2015 
// Design Name: 
// Module Name:    CIC_logsys 
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
	input clk,             // clock
	input rst,             // reset
	input din,             // data
	input [2:0] comb_num,  // comb's rate
	input [15:0] dec_num,  // decimator's rate
	output reg [31:0] out, // CIC output
	output reg out_rdy     // output valid
   );

reg [31:0] integ;
reg [15:0] dec_cntr;

reg [31:0] comb [7:0];
integer i;

always@(posedge clk)
begin

if(rst)
begin
	integ <= 32'd0;
	dec_cntr <= 16'd0;
	for(i = 0; i < 8; i = i+1)
	begin
		comb[i] <= 32'd0;
	end
	out_rdy <= 1'b0;
end
else
begin
	
	//integrator
	integ <= integ + din;
	//decimator
	dec_cntr <= dec_cntr + 1;
	if(dec_cntr == dec_num)
	begin
		//comb
		comb[0] <= integ;
		//shift
		for(i = 1; i < 8; i = i+1)
		begin
			comb[i] <= comb[i-1];
		end
		//selecting based on comb's rate
		case (comb_num)
		3'd0 : out <= integ - comb[0];
		3'd1 : out <= integ - comb[1];
		3'd2 : out <= integ - comb[2];
		3'd3 : out <= integ - comb[3];
		3'd4 : out <= integ - comb[4];
		3'd5 : out <= integ - comb[5];
		3'd6 : out <= integ - comb[6];
		3'd7 : out <= integ - comb[7];
		endcase

		out_rdy <= 1'b1;
		dec_cntr <= 16'd0;
	end
	else
	begin
		out_rdy <= 1'b0;
	end
end

end // always
endmodule
