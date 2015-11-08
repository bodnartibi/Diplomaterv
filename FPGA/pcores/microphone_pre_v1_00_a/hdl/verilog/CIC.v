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
	
	input [4:0] comb_num,       // comb's rate
	input [7:0] dec_num,       // decimator's rate
	output reg [31:0] data_out, // CIC filter output
	output reg data_out_valid,  // output valid
	
	output channel,             // 
	output reg clk_out,         // clock to microphone 1 MHz
	input data_in               // data from the microphone (based on clok_out)
   );

reg [31:0] integ;
reg [7:0] dec_cntr;
reg [31:0] comb [31:0]; // regiszer hossz, regiszter darabszam
integer i;

reg local_valid;
reg local_valid_state;

// channel right/left_negalt
// right channel --> felfut��lre kezd el megjelenni az adat
wire right_channel = 1'b1;
assign channel = right_channel;

reg [13:0] clk_counter = 13'd0;
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
		end
	else
		begin
			if (clk_div_tc)
				begin
				clk_out <= ~ clk_out;
				end
			if(clk_counter == 32'd24)
				begin
				clk_div_tc <= 1'b1;
				clk_counter <= 32'd0;
				end
			else
				begin
				clk_div_tc <= 1'b0;
				clk_counter <= clk_counter + 1;
				end
			
			if(local_valid && !local_valid_state)
				begin
				data_out_valid <= 1'b1;
				local_valid_state <= 1'b1;
				end
			else if(local_valid && local_valid_state)
				begin
				data_out_valid <= 1'b0;
				end
			else
				begin
				data_out_valid <= 1'b0;
				local_valid_state <=1'b 0;
				end
		end
end

always@(posedge clk)
begin

	if(rst)
		begin
			integ <= 32'd0;
			dec_cntr <= 8'd0;
			for(i = 0; i < 32; i = i+1)
				comb[i] <= 31'd0;
			data_out <= 15'd0;
			local_valid <= 1'b0;
		end
	else if(clk_out_fall)
		begin
	
			//integrator
			integ <= integ + data_in;
			//decimator
			dec_cntr <= dec_cntr + 1;
			if(dec_cntr == dec_num)
				begin
					//comb
					comb[0] <= integ;
					//shift
					for(i = 1; i < 32; i = i+1)
						comb[i] <= comb[i-1];
					//selecting based on comb's rate
					case (comb_num)
						5'd0 : data_out <= integ - comb[0];
						5'd1 : data_out <= integ - comb[1];
						5'd2 : data_out <= integ - comb[2];
						5'd3 : data_out <= integ - comb[3];
						5'd4 : data_out <= integ - comb[4];
						5'd5 : data_out <= integ - comb[5];
						5'd6 : data_out <= integ - comb[6];
						5'd7 : data_out <= integ - comb[7];
						5'd8 : data_out <= integ - comb[8];
						5'd9 : data_out <= integ - comb[9];
						5'd10: data_out <= integ - comb[10];
						5'd11: data_out <= integ - comb[11];
						5'd12: data_out <= integ - comb[12];
						5'd13: data_out <= integ - comb[13];
						5'd14: data_out <= integ - comb[14];
						5'd15: data_out <= integ - comb[15];
						5'd16: data_out <= integ - comb[16];
						5'd17: data_out <= integ - comb[17];
						5'd18: data_out <= integ - comb[18];
						5'd19: data_out <= integ - comb[19];
						5'd20: data_out <= integ - comb[20];
						5'd21: data_out <= integ - comb[21];
						5'd22: data_out <= integ - comb[22];
						5'd23: data_out <= integ - comb[23];
						5'd24: data_out <= integ - comb[24];
						5'd25: data_out <= integ - comb[25];
						5'd26: data_out <= integ - comb[26];
						5'd27: data_out <= integ - comb[27];
						5'd28: data_out <= integ - comb[28];
						5'd29: data_out <= integ - comb[29];
						5'd30: data_out <= integ - comb[30];
						5'd31: data_out <= integ - comb[31];
					endcase

					local_valid <= 1'b1;
					dec_cntr <= 8'd0;
				end
			else
				begin
					local_valid <= 1'b0;
				end
		end

end // always
endmodule

