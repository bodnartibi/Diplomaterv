`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:48:26 09/06/2015 
// Design Name: 
// Module Name:    Buffer 
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

parameter DATA_WIDTH	= 9;
parameter BUFFER_SIZE = 16;
//integer k;

module Buffer(
    // bufferba mentendo adat
    input [DATA_WIDTH - 1:0] data_in,
    input data_in_valid,
	 output reg data_in_ack,
	 // bufferbol kiolvasas
	 output reg [DATA_WIDTH - 1:0] data_out,
	 // van adat a bufferban
	 output reg data_out_valid,
	 // olvasnak a buffer
	 input data_out_read,
	 // az adat a kimeneten
	 output reg data_out_read_ack,
	 input rst,
	 input clk
    );

// a szelessegeken igazitani kell,
// pl cntr szelessegeket
reg [DATA_WIDTH - 1:0] buff [0: BUFFER_SIZE -1];

reg [BUFFER_SIZE -1:0] cntr_first;
reg [BUFFER_SIZE -1:0] cntr_last;
reg [BUFFER_SIZE -1:0] cntr_of_valid_data;

always@(posedge clk)
begin
	if(rst)
	begin
		// inicializalas
		data_out <= 0;
		data_out_valid <= 0;
		data_out_read_ack <= 0;
		cntr_first <= 0;
		cntr_last <= 0;
		cntr_of_valid_data <= 0;
		//for (k = 0; k < BUFFER_SIZE - 1; k = k + 1)
		//begin
		//	buff[k] = 0;
		//end
		
	end
	// ha van új bejövõ adat
	// ekkor nem adunk ki adatot, de lehet elozo ciklusban igen
	// igy itt a kimeno jel valid vonalt nullaznunk kell
	
	// input oldalnak elsobbsege van
	else if(data_in_valid)
	begin
		buff[cntr_last] <= data_in;
		cntr_last <= cntr_last + 1;
		data_in_ack <= 1'b1;
		cntr_of_valid_data <= cntr_of_valid_data + 1;
		
		// kiadott jelekre vonatkozo dolgok
		// elozo ciklusban lehet adtunk az output oldalra
		// ezert itt nullazzuk
		data_out_valid <= 1'b0;
	end
	
	// ack tema meg hatra van, azt meg kellene varni
	else if(cntr_of_valid_data > 0)
	begin
		data_out_valid <= 1'b1;
		data_out <= buff[cntr_first];
	end
	
	// nincs semmi, ack stb jeleket nullara kell hozni 
	else
	begin
		data_out_valid <= 1'b0;
		data_out_read_ack <= 1'b0;
		data_out <= 0;
	end
	
	
end

endmodule
