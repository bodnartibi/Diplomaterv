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
	output reg [31:0] timer_out,
	output reg timer_valid,
	input ack
);

// felkeszulunk arra, ha nem ugyanarrol az orajelrol megy
// az a modul, illetve a Threshold modul
// ekkor lehet, hogy a detect jel a tobb orajelig is magas

reg [31:0] timer;
reg [31:0] prev;

// a timernek addig kell tartania az erteket,
// amig az nem nyugtazodik a masik oldalrol

always@(posedge clk)
begin
	if(rst)
		begin
			timer_valid <= 1'b0;
			timer_out <= 32'd0;
			timer <= 32'd0;
			prev <= 32'd0;
		end
	else
		begin
			timer <= timer + 1;
		// ha uj detect jel jott
		// ha ez meg mindig az elozo,
		// akkor nem frissitjuk a kimenetet
		if(detect == 1'b1 && timer_valid == 1'b0)
		begin
		   if( (timer - prev) > 32'd5000000 )
			begin
				timer_valid <= 1'b1;
				timer_out <= timer;
			end
			prev <= timer;
		end
		// nyugtaztak a kimenetet,
		// ezutan varhatjuk a kovetkezo detect jelet
		else if (timer_valid == 1'b1 && ack == 1'b1)
		begin
			timer_valid <= 1'b0;
		end
		end
end

endmodule
