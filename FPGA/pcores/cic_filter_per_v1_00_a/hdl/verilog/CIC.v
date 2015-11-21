`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:11:36 10/24/2015 
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
	input clk,                  // clock in 50 MHz
	input rst,                  // reset
	input [31:0] clk_div,       // x eseten (x+1)*2 az osztas
	
	//input [5:0] comb_num,       // comb's rate
	input [7:0] dec_num,       // decimator's rate
	output reg [31:0] data_out, // CIC filter output
	output reg data_out_valid,  // output valid
	
	output channel,             // 
	output reg clk_out,         // clock to microphone 1 MHz
	input data_in               // data from the microphone (based on clok_out)
   );

reg [31:0] integ;
reg [7:0] dec_cntr;
reg [31:0] comb [63:0]; // regiszer hossz, regiszter darabszam
integer i;

reg local_valid;
reg local_valid_state;

// channel right/left_negalt
// right channel --> felfutóélre kezd el megjelenni az adat
wire right_channel = 1'b1;
assign channel = right_channel;

reg [31:0] clk_counter;
reg clk_div_tc;
wire clk_out_ris;
wire clk_out_fall;
//clk falling, rising
assign clk_out_ris = clk_div_tc && !clk_out;
assign clk_out_fall = clk_div_tc && clk_out;

always @(posedge clk)
begin
	if (rst)
		begin
			clk_counter <= 32'd0;
			clk_out <= 1'b1;
			clk_div_tc <= 1'b1;
			local_valid_state <= 1'b0;
			data_out_valid <= 1'b0;
		end
	else
		begin
			if (clk_div_tc)
				begin
				clk_out <= ~ clk_out;
				end
			if(clk_counter == clk_div)
				begin
				clk_div_tc <= 1'b1;
				clk_counter <= 32'd0;
				end
			else
				begin
				clk_div_tc <= 1'b0;
				clk_counter <= clk_counter + 1;
				end

			// egy orajelig legyen aktiv a CIC adat valid jele
			if(local_valid && !local_valid_state)
				begin
				local_valid_state <= 1'b1;
				end
			else if(!local_valid && local_valid_state)
				begin
				data_out_valid <= 1'b1;
				local_valid_state <=1'b0;
				end
			else
				begin
				data_out_valid <= 1'b0;
				end
		end
end

always@(posedge clk)
begin

	if(rst)
		begin
			integ <= 32'd0;
			dec_cntr <= 8'd0;
			//for(i = 0; i < 63; i = i+1)
			//	comb[i] <= 32'd0;
			data_out <= 32'd0;
			local_valid <= 1'b0;
		end
	else if(clk_out_fall)
		begin
	
			//integrator
			integ <= integ + data_in;
			//decimator
			if(dec_cntr == dec_num)
				begin
					//comb
					comb[0] <= integ;
					//shift
					for(i = 1; i < 64; i = i+1)
					begin
						comb[i] <= comb[i-1];
					end
					//constant 64 div
					data_out <= integ - comb[63];

					local_valid <= 1'b1;
					dec_cntr <= 8'd0;
				end
			else
				begin
					dec_cntr <= dec_cntr + 1;
					local_valid <= 1'b0;
				end
		end

end // always
endmodule

