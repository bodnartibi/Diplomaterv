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
	
	input [5:0] comb_num,       // comb's rate
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
			for(i = 0; i < 64; i = i+1)
				comb[i] <= 31'd0;
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
						comb[i] <= comb[i-1];
					//selecting based on comb's rate
					case (comb_num)
						6'd0 : data_out <= integ - comb[0];
						6'd1 : data_out <= integ - comb[1];
						6'd2 : data_out <= integ - comb[2];
						6'd3 : data_out <= integ - comb[3];
						6'd4 : data_out <= integ - comb[4];
						6'd5 : data_out <= integ - comb[5];
						6'd6 : data_out <= integ - comb[6];
						6'd7 : data_out <= integ - comb[7];
						6'd8 : data_out <= integ - comb[8];
						6'd9 : data_out <= integ - comb[9];
						6'd10: data_out <= integ - comb[10];
						6'd11: data_out <= integ - comb[11];
						6'd12: data_out <= integ - comb[12];
						6'd13: data_out <= integ - comb[13];
						6'd14: data_out <= integ - comb[14];
						6'd15: data_out <= integ - comb[15];
						6'd16: data_out <= integ - comb[16];
						6'd17: data_out <= integ - comb[17];
						6'd18: data_out <= integ - comb[18];
						6'd19: data_out <= integ - comb[19];
						6'd20: data_out <= integ - comb[20];
						6'd21: data_out <= integ - comb[21];
						6'd22: data_out <= integ - comb[22];
						6'd23: data_out <= integ - comb[23];
						6'd24: data_out <= integ - comb[24];
						6'd25: data_out <= integ - comb[25];
						6'd26: data_out <= integ - comb[26];
						6'd27: data_out <= integ - comb[27];
						6'd28: data_out <= integ - comb[28];
						6'd29: data_out <= integ - comb[29];
						6'd30: data_out <= integ - comb[30];
						6'd31: data_out <= integ - comb[31];			
						6'd32: data_out <= integ - comb[32];
						6'd33: data_out <= integ - comb[33];
						6'd34: data_out <= integ - comb[34];
						6'd35: data_out <= integ - comb[35];
						6'd36: data_out <= integ - comb[36];
						6'd37: data_out <= integ - comb[37];
						6'd38: data_out <= integ - comb[38];
						6'd39: data_out <= integ - comb[39];
						6'd40: data_out <= integ - comb[40];
						6'd41: data_out <= integ - comb[41];
						6'd42: data_out <= integ - comb[42];
						6'd43: data_out <= integ - comb[43];
						6'd44: data_out <= integ - comb[44];
						6'd45: data_out <= integ - comb[45];
						6'd46: data_out <= integ - comb[46];
						6'd47: data_out <= integ - comb[47];
						6'd48: data_out <= integ - comb[48];
						6'd49: data_out <= integ - comb[49];
						6'd50: data_out <= integ - comb[50];
						6'd51: data_out <= integ - comb[51];
						6'd52: data_out <= integ - comb[52];
						6'd53: data_out <= integ - comb[53];
						6'd54: data_out <= integ - comb[54];
						6'd55: data_out <= integ - comb[55];
						6'd56: data_out <= integ - comb[56];
						6'd57: data_out <= integ - comb[57];
						6'd58: data_out <= integ - comb[58];
						6'd59: data_out <= integ - comb[59];
						6'd60: data_out <= integ - comb[60];
						6'd61: data_out <= integ - comb[61];
						6'd62: data_out <= integ - comb[62];
						6'd63: data_out <= integ - comb[63];
					endcase

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

