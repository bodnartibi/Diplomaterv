`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:08:30 04/12/2015 
// Design Name: 
// Module Name:    Timer 
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
module Timer(
    input clk,
    input rst,
    input detect,
    output reg [31:0] timer_value,
	 output reg timer_valid
    );

// felkeszulunk arra, ha nem ugyanarrol az orajelrol megy
// az a modul, illetve a Threshold modul
// ekkor lehet, hogy a detect jel a tobb orajelig is magas

reg new_detect;

always@(posedge clk)
begin
	if(rst)
		begin
		new_detect <= 1'b0;
		timer_valid <= 1'b0;
		timer_value <= 32'd0;
		end
	else
		begin
		timer_value <= timer_value + 1;
		if(detect)
			begin
			if(new_detect == 1'b0)
				begin
				new_detect <= 1'b1;
				timer_valid <= 1'b1;
				end
			else
				begin
				timer_valid <= 1'b0;
				end
			end
		else
			begin
				new_detect <= 1'b0;
				timer_valid <= 1'b0;
			end
		end
end

endmodule
