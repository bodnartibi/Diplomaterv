`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:20:40 04/12/2015 
// Design Name: 
// Module Name:    Threshold 
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

// TODO a hossz itt is vmi definebol jojjon
//parameter HIGH	=	10'd800;
//parameter LOW	=	10'd400;
//ez
parameter STATE_HIGH =	1'b1;
parameter STATE_LOW	=	1'b0;

module Threshold(
    input [31:0] cntr,
    input cntr_valid,
	 input rst,
	 input clk,
	 input [31:0] HIGH,
	 input [31:0] LOW,
    output reg detect
    );

reg state;

always@(posedge clk)
begin
	if(rst)
		begin
		state <= STATE_LOW;
		detect <= 1'b0;
		end
	else
		begin
		if(state == STATE_LOW)
			begin
			if(cntr >= HIGH && cntr_valid)
				begin
				state <= STATE_HIGH;
				detect <= 1'b1;
				end
			end
		
		if(state == STATE_HIGH)
			begin
			// csak egy orajelig magas a detect
			detect <= 1'b0;
			if(cntr <= LOW  && cntr_valid)
				state <= STATE_LOW;
			end
		end
end

endmodule
