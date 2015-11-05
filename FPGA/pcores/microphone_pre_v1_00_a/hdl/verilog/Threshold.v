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

module Threshold(
		input [31:0] data,
		input data_valid,
		input rst,
		input clk,
		input [31:0] HIGH,
		input [31:0] LOW,
		input ack,
		output reg valid,
		output reg [31:0] detect_time;
    );

reg [31:0] timer_cntr;
reg [31:0] timer;
reg [31:0] max_value;
reg [31:0] max_value_time;
reg [5:0] zero_cntr;
wire signal;
// csak akkor erdekel minket a jel, ha az valid es hatarerteken kivul van
assign signal = ((data > HIGH) || (data < LOW)) && data_valid;

always@(posedge clk)
begin
	if(rst)
		begin
		state <= 2'd0;
		valid <= 1'b0;
		timer <= 32'd0;
		timer_cntr <= 32'd0;
		max_value <= 32'd0;
		max_value_time <= 32'd0;
		zero_cntr <= 6'd51;
		detect_time <= 32'd0;
		end

	else
/*
		begin
		// CIC 50MHz-et kap, de 1MHz-el jönnek az adatok, azt 16-odra ecimalja
		// azaz a cic az 50MHz 800-ad reszevel ad ki adatot (50*16=800)
		if(timer_cntr == 32'd799)
		begin
			timer_cntr <= 32'd0;
			timer <= timer + 1;
		end
		else
		begin
			timer_cntr <= timer_cntr + 1;
		end
*/

		// a szuro kimenete leptesse az idot
		if(data_valid)
		begin
			timer <= timer + 1;
		end


		// output kiadasa
		if(zero_cntr == 6'd50)
		begin
			detect_time <= max_value_time;
			valid <= 1'b1;
			max_value <= 32'd0;
			// varjunk amig nem jon ack
			if(ack)
			begin
				zero_cntr <= zero_cntr + 1;
			end
		end
		// varakozas tapsra
		if(!signal && zero_cntr > 6'd50)
		begin
			valid <= 1'b0;
		end
		// tapsban vagyunk, jott uj jel
		else if (signal)
		begin
			zero_cntr <= 32'd0;
			if(data > max_value)
			begin
				max_value <= data;
				max_value_time <= timer;
			end
		end
		// tapsbol tartunk kifele, nem jott jel
		else if (!signal && (zero_cntr < 6'd50))
		begin
			zero_cntr <= zero_cntr + 1 ;
		end

end
endmodule
