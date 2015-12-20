/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * 
 *
 * This file is a generated sample test application.
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * SDK application project when you run the "Generate Libraries" menu item.
 *
 */


#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "uartlite_header.h"


#define HIGH_1 395
#define HIGH_2 405
#define MEM_SLOT_SIZE 0xFFFFF // 1MByte
#define SAFETY_OFFSET 0x10000 //

#define MEM_SLOT_0 (XPAR_LOGSYS_AXI_SDRAM_CTRL_0_S_AXI_BASEADDR + SAFETY_OFFSET)
#define MEM_SLOT_1 (MEM_SLOT_0 + SAFETY_OFFSET + MEM_SLOT_SIZE)
#define MEM_SLOT_2 (MEM_SLOT_1 + SAFETY_OFFSET + MEM_SLOT_SIZE)

#define CUT_NUM 5000

void sleep(int num)
{
	int i;
	for (i = 0; i < XPAR_CPU_CORE_CLOCK_FREQ_HZ/5*num; i++);
}

int main() 
{

    /*
     * CIC registers:
     * data:   BASE_ADDRESS
     * status: BASE_ADDRESS + 0x4
     */

   Xil_ICacheEnable();
   Xil_DCacheEnable();

   const u32* cic_data_p[3] = { XPAR_CIC_FILTER_PER_0_BASEADDR, \
                                XPAR_CIC_FILTER_PER_1_BASEADDR, \
                                XPAR_CIC_FILTER_PER_2_BASEADDR };
   const u32* cic_status_p[3] = { XPAR_CIC_FILTER_PER_0_BASEADDR + 0x4, \
                                  XPAR_CIC_FILTER_PER_1_BASEADDR + 0x4, \
                                  XPAR_CIC_FILTER_PER_2_BASEADDR + 0x4 };
   int num[3] = {0,0,0};

   u32* act_mem_p[3] = { MEM_SLOT_0, MEM_SLOT_1, MEM_SLOT_2 };
   u32* end_mem_p[3] = { MEM_SLOT_0 + MEM_SLOT_SIZE, MEM_SLOT_1 + MEM_SLOT_SIZE, MEM_SLOT_2 + MEM_SLOT_SIZE};

   int i;

   print("---Entering main---\n\r");


   /*
    * Peripheral SelfTest will not be run for axi_uartlite_0
    * because it has been selected as the STDOUT device
    */

   {
      int status;
      
      print("\r\nRunning UartLiteSelfTestExample() for debug_module...\r\n");
      status = UartLiteSelfTestExample(XPAR_DEBUG_MODULE_DEVICE_ID);
      if (status == 0) {
         print("UartLiteSelfTestExample PASSED\r\n");
      }
      else {
         print("UartLiteSelfTestExample FAILED\r\n");
      }
   }

   print("---Waiting for valid datas and collecting them---\n\r");

   // Waiting for first valid value
   while(1){
       if(*cic_status_p[0] && (*cic_data_p[0] > HIGH_1)){
           break;
       }
       if(*cic_status_p[1] && (*cic_data_p[1] > HIGH_1)){
           break;
       }
       if(*cic_status_p[2] && (*cic_data_p[2] > HIGH_1)){
           break;
       }
   }
   // Collecting

   while((act_mem_p[0] < end_mem_p[0]) && \
		 (act_mem_p[1] < end_mem_p[1]) && \
		 (act_mem_p[2] < end_mem_p[2])){

       if(*cic_status_p[0]) {
          *act_mem_p[0] = *cic_data_p[0];
          act_mem_p[0]++;
       }

       if(*cic_status_p[1]) {
          *act_mem_p[1] = *cic_data_p[1];
          act_mem_p[1]++;
       }
       if(*cic_status_p[2]) {
           *act_mem_p[2] = *cic_data_p[2];
           act_mem_p[2]++;
       }
   }

   print("---End of collecting, cutting invalid datas from the end---\n\r");


   end_mem_p[0] = act_mem_p[0];
   end_mem_p[1] = act_mem_p[1];
   end_mem_p[2] = act_mem_p[2];

   act_mem_p[0] = MEM_SLOT_0;
   act_mem_p[1] = MEM_SLOT_1;
   act_mem_p[2] = MEM_SLOT_2;

   for(i = 0; i < 3; i++){
	   while(1) {
		   if((num[i] > CUT_NUM) || (act_mem_p[i] >= end_mem_p[i])) {
			   end_mem_p[i] = act_mem_p[i];
			   break;
		   }
		   if(*act_mem_p[i] < HIGH_2){
			   num[i]++;
		   } else {
			   num[i] = 0;
		   }
		   act_mem_p[i]++;
	   }
   }

   print("Soon...\n\r");
   act_mem_p[0] = MEM_SLOT_0;
   act_mem_p[1] = MEM_SLOT_1;
   act_mem_p[2] = MEM_SLOT_2;

   for(i = 0; i < 3; i++) {
	   sleep(2);
	   print("---MIC ");
	   putnum(i);
	   print(" \n\r");

	   while(act_mem_p[i] <= end_mem_p[i]){
		   putnum(*act_mem_p[i]);
		   print("\n\r");
		   act_mem_p[i]++;
	   }
   }

   print("---Exiting main---\n\r");

   Xil_DCacheDisable();
   Xil_ICacheDisable();

   return 0;
}

