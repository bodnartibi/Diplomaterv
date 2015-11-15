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
		output reg [31:0] detect_time
    );

reg [31:0] timer;
reg [31:0] max_value;
reg [31:0] zero_cntr;
reg [1:0] state;

always@(posedge clk)
begin
	if(rst)
		begin
		valid <= 1'b0;
		timer <= 32'd0;
		max_value <= 32'd0;
		zero_cntr <= 32'd0;
		detect_time <= 32'd0;
		state <= 2'd0;
		end

	else

		// a szuro kimenete leptesse az idot
		if(data_valid)
		begin
			timer <= timer + 1;
		end


		case (state)
		// nem ablakban vagyunk
		2'd0 :
			begin
				valid <= 1'b0;
				zero_cntr <= 32'd0;
				if((data > HIGH) && data_valid)
				begin
					max_value <= data;
					detect_time <= timer;
					state <= 2'd1;
				end
				else
				begin
					max_value <= 32'd0;
				end
			end
		// ablakban vagyunk
		2'd1 :
			begin
				// jott uj jel idon belul,
				// ujrakezdj�k a zero szamlalast
				// frissitjuk a legnagyobb ertekeket, ha kell
				if((data > HIGH) && data_valid)
				begin
					zero_cntr <= 32'd0;
					if (data > max_value)
					begin
						max_value <= data;
						detect_time <= timer;
					end
				end
				// nem jott jel, szamolunk, es ellen�rizzuk
				else if (data_valid)
				begin
					zero_cntr <= zero_cntr + 1;
					// ha mar eleg nulla jott
					if(zero_cntr >= 32'd10000)
					begin
						valid <= 1'b1;
						max_value <= 32'd0;
						state <= 2'd0;
					end
				end
			end
		endcase

end
endmodule
