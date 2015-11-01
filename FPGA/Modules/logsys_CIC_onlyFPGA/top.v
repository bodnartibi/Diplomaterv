`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:31:40 10/23/2015 
// Design Name: 
// Module Name:    top 
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
module top(
	 output cpld_jtagen,
	 output cpld_rstn,
	 output cpld_clk,
	 output cpld_load,
	 output cpld_mosi,
	 input rstn_in,
	 input clk,
	 input cpld_miso,
	 // microphone
	output mic_channel,
	output mic_clk_out,
	input mic_data_in
    );

wire [7:0] SW;
wire [4:0] NAV;

wire [31:0] data;
wire data_valid;

reg [7:0] event_cntr;
reg event_state;

//temp
//assign mic_channel = 1'b0;
//assign mic_clk_out = 1'b0;
//assign data = 32'd0;
//assign data_valid = 1'b0;

cpld_per cpld_per(
	 .cpld_jtagen(cpld_jtagen),
	 .cpld_rstn(cpld_rstn),
	 .cpld_clk(cpld_clk),
	 .cpld_load(cpld_load),
	 .cpld_mosi(cpld_mosi),
	 .SW(SW),
	 .NAV(NAV),
	 .cpld_miso(cpld_miso),
	 .clk50(clk),
	 .LEDs_in(event_cntr),
	 .disp1_in(SW),
	 .disp2_in(SW),
	 .rstn_in(rstn_in)
    );


CIC_logsys CIC(
	.clk(clk),                  // clock in 50 MHz
	.rst(!rstn_in),                  // reset
	
	.comb_num(3'd7),       // comb's rate
	.dec_num(16'd75),       // decimator's rate
	.data_out(data), // CIC filter output
	.data_out_valid(data_valid),  // output valid
	
	.channel(mic_channel),             // 
	.clk_out(mic_clk_out),         // clock to microphone 1 MHz
	.data_in(mic_data_in)              // data from the microphone (based on clok_out)
   );



always @(posedge clk)
begin
	if(!rstn_in)
	begin
		event_cntr <= 8'd0;
		event_state <= 1'b0;
	end
	else
	begin
		if(data_valid)
		begin
			if(data >= 32'd550 && !event_state)
			begin
				event_cntr <= event_cntr + 1;
				event_state <= 1'b1;
			end
			else if(data <= 32'd250 && event_state)
			begin
				event_state <= 1'b0;
			end
		end
	end
end

endmodule
