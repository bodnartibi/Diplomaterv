`timescale 1ns / 1ps

module CIC(
	input clk,                  // bejovo orajel (50 MHz)
	input rst,                  // reszet
	input [4:0] clk_div,        // x eseten (x+1)*2 az osztas
	
	//input [5:0] comb_num,     // atlagolas merteke
	input [3:0] dec_num,        // decimalas mertek
	output reg [31:0] data_out, // kimenet
	output reg data_out_valid,  // kimenet ervenyessege
	
	output channel,             // 
	output reg clk_out,         // mikrofon orajele
	input data_in               // mikrofontol jovo adat
   );

reg [24:0] integ;
reg [3:0] dec_cntr;
reg [24:0] comb [127:0]; // regiszer hossz, regiszter darabszam
integer i;

reg local_valid;
reg local_valid_state;

// channel right/left_negalt
// right channel --> felfut��lre kezd el megjelenni az adat
wire right_channel = 1'b1;
assign channel = right_channel;

reg [4:0] clk_counter;
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
			clk_counter <= 5'd0;
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
				clk_counter <= 5'd0;
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
			integ <= 24'd0;
			dec_cntr <= 4'd0;
			for(i = 0; i < 128; i = i+1)
				comb[i] <= 25'd0;
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
					for(i = 1; i < 128; i = i+1)
					begin
						comb[i] <= comb[i-1];
					end
					//constant 128 div
					data_out <= {8'd0, integ - comb[127]};

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

