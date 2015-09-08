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

parameter DATA_WIDTH	= 32;
parameter BUFFER_SIZE = 16;
parameter COUNTER_SIZE = 4;
//integer k;

// bejovo oldalon van nyugtaz
// kimeno oldalnak elsobbsege van,
// azt 1 orajel alatt kiszolgaljuk
// buffer beteltekor nem adunk nyugtat
// a bejovo adatra
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
	 
	 input rst,
	 input clk
    );

// TODO a szelessegeken igazitani kell,
// pl cntr szelessegeket
reg [DATA_WIDTH - 1:0] buff [0: BUFFER_SIZE -1];

reg [COUNTER_SIZE -1:0] cntr_first;
reg [COUNTER_SIZE -1:0] cntr_last;
reg [COUNTER_SIZE -1:0] cntr_of_valid_data;

always@(posedge clk)
begin
	if(rst)
	begin
		// inicializalas
		data_out <= 0;
		data_out_valid <= 0;
		cntr_first <= 0;
		cntr_last <= 0;
		cntr_of_valid_data <= 0;
		data_in_ack <= 0;
		//for (k = 0; k < BUFFER_SIZE - 1; k = k + 1)
		//begin
		//	buff[k] = 0;
		//end
		
	end
	// ha van új bejövõ adat
	// ekkor nem adunk ki adatot, de lehet elozo ciklusban igen
	// igy itt a kimeno jel valid vonalt nullaznunk kell
	
	// --- Output oldalon kiolvasta a masik fel
	// data_out_read jel jelzi,
	// persze csak akkor, ha van is mit kiolvasni
	// legnagyobb prioritasu, hiszen igy tudunk
	// uriteni a bufferbol
	// mashol nem nullazzuk a valid jelet,
	// csak itt szabad, hiszen csak ekkor olvassak ki
	else if(data_out_read && cntr_of_valid_data > {{COUNTER_SIZE{1'b0}}})
	begin
		// kevesebb adat maradt a bufferban
		// TODO tulcsordulas
		cntr_first <= cntr_first + 1;
		// mindenkepp eggyel kevesebb adatunk van mar csak
		cntr_of_valid_data <= cntr_of_valid_data - 1;
		
		// ha van meg adat, akkor kirakjuk azt
		// ha nincs, akkor levesszuk a kimenetet
		if(cntr_of_valid_data > {{COUNTER_SIZE-1{1'b0}}, 1'b1})
		begin
		//TODO tulcsordulas
			data_out <= buff[cntr_first + 1];
		end
		else
		begin
			data_out <= 0;
			data_out_valid <= 1'b0;
		end		
		// data_in_valid lehetett az elozo ciklusban
		data_in_ack <= 1'b0;
	end
	
	// --- Input oldalon szeretnenek beirni
	// masodik legfontosabb
	// data_in_valid jelzi
	else if(data_in_valid && cntr_of_valid_data != {COUNTER_SIZE{1'b1}})
	begin
		buff[cntr_last] <= data_in;
		// TODO tulcsordulas
		cntr_last <= cntr_last + 1;
		// Ezt mindenhol mashol kell lehuznunk nullaba
		data_in_ack <= 1'b1;
		cntr_of_valid_data <= cntr_of_valid_data + 1;
	end
	
	// --- Ha van a bufferben adat
	// a fenti esetet nem rontja el,
	else if(cntr_of_valid_data > 0)
	begin
		data_out_valid <= 1'b1;
		data_out <= buff[cntr_first];
		// data_in_valid lehetett az elozo ciklusban
		data_in_ack <= 1'b0;
	end

	// nincs semmi, ack stb jeleket nullara kell hozni 
	else
	begin
		// data_in_valid lehetett az elozo ciklusban
		data_in_ack <= 1'b0;
	end
	
	
end

endmodule
