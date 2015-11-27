BUS_frek_kHz = 50000; % kHz
BUS_to_MIC_div = 16;
DEC_num = 6;
COMB_num = 64;
HANG_V = 343;  %mm/kHz

mic_frek = BUS_frek_kHz/BUS_to_MIC_div;
CIC_out_frek = mic_frek/ DEC_num
ido_kvantum = 1/CIC_out_frek;
tavolsag = ido_kvantum*HANG_V
vagasi_frek = CIC_out_frek/COMB_num