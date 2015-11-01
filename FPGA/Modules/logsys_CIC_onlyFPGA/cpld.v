`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:26:12 02/05/2014 
// Design Name: 
// Module Name:    cpld_per 
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
module cpld_per(
	 output cpld_jtagen,
	 output cpld_rstn,
	 output reg cpld_clk,
	 output cpld_load,
	 output cpld_mosi,
	 output reg[7:0] SW,
	 output reg[4:0] NAV,
	 input cpld_miso,
	 input clk50,
	 input [7:0] LEDs_in,
	 input [7:0] disp1_in,
	 input [7:0] disp2_in,
	 input rstn_in
    );

//local
reg [13:0] clk_counter = 0;
reg clk_div_tc;
wire cpld_clk_rise;
wire cpld_clk_fall;

reg [15:0] in_shift;
reg [15:0] out_shift;

reg [7:0] disp;
reg load;
reg disp_switch = 0;

reg [3:0] over_all_state = 0;

//clk falling, rising
assign cpld_clk_rise = clk_div_tc && !cpld_clk;
assign cpld_clk_fall = clk_div_tc && cpld_clk;

assign cpld_jtagen = 0;
assign cpld_rstn = rstn_in;
assign cpld_load = load;
assign cpld_mosi = out_shift[15];

//clk div

always @(posedge clk50)
begin
	if (!rstn_in)
		begin
			clk_counter <= 0;
			cpld_clk <= 0;
			clk_div_tc <= 0;
		end
	else
		begin
			clk_counter <= clk_counter + 1;
			if(clk_counter == 14'd7813)
				begin
				clk_div_tc <= 1;
				clk_counter <= 0;
				end
			else if (clk_div_tc)
				begin
				cpld_clk <= ~ cpld_clk;
				clk_div_tc <= 0;
				end
		end
end


//disp switch
always @(posedge clk50)
begin
	if(!rstn_in)
	begin
		over_all_state <= 4'd14;
		disp <= 8'd0;
	end
	else if (cpld_clk_rise)
	begin
		over_all_state <= over_all_state + 1;
		if(over_all_state == 4'd0)
		begin
			case(disp_switch)
				0: disp[7:0] <= disp1_in[7:0];
				1: disp[7:0] <= disp2_in[7:0];		
			endcase
		end
	end
end


//load signal
always @(posedge clk50)
begin
	if(!rstn_in)
		load <= 1'b0;
	else if (cpld_clk_fall)
	begin
		if (over_all_state == 4'd15)
			load <= 1;	
		else
			load <= 0;
	end
end

//out shift in case fall
always @(posedge clk50)
begin
	if(!rstn_in)
		out_shift <= 16'd0;
	else if(cpld_clk_fall)
	begin
		if (load)
			out_shift <= {LEDs_in[7:0],disp[7:0]};
		else
			out_shift <= {out_shift[14:0],out_shift[15]};
	end
end

//in shift in case fall
always @(posedge clk50)
begin
	if(!rstn_in)
		in_shift <= 16'd0;
	else if(cpld_clk_fall)
	begin
		if (load)
			begin
				SW [7:0] <= in_shift[15:8];
				NAV [4:0] <= in_shift[7:3];
				disp_switch <= in_shift[2];
			end
		in_shift <= {in_shift[14:0],cpld_miso};
	end
end

endmodule