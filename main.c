//------------------------------------------------------------------------------
//      
//  File Name: main.c
//
//  Description: This file contains the Main Routine - "While" Operating System,
//               handles calling initialization functions, calling the BL 
//               movement state machine, as well as handling serial processing
//               flags
//
//  Alex Daum
//  Sept 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include "ports.h"
#include <string.h>


  // Global Variables
volatile char slow_input_down;
extern char display_line[DISPLAY_LINE_TOTAL][SCREEN_LIMIT_END];
extern char *display[DISPLAY_LINE_TOTAL];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
unsigned int test_value;

extern volatile char sec_flag;
unsigned char onetime = ON;
unsigned char IOT_AUTO_active_flag;
extern volatile unsigned int usb_rx_wr;

char proc_buff_hold[DBL_RING_SIZE];

//UCA0 =========================================================================
extern volatile unsigned char UCA0_read_flag;
extern volatile unsigned char UCA0_process_flag;
extern volatile unsigned char UCA0_transmit_flag;

extern volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
unsigned char UCA0_proc_buff[DBL_RING_SIZE];
extern volatile unsigned int iot_rx_wr;
extern unsigned int usb_rx_rd;
unsigned int UCA0_proc_index;

extern volatile unsigned char PC_2_IOT[SMALL_RING_SIZE];
extern volatile unsigned int usb_rx_wr;
extern unsigned int iot_rx_rd;

//==============================================================================
// Function Name: main
//
// Description: Main routine for the program, code execution begins here.
//              handles initialization, calling primary state machine functions
//              and processing serial flag needs
//
// Note: Operating system is Background/Foreground
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions                          /
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_Serial_UCA0();                  // Initialize UCA0 to Baudrate 115,200

// Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[DISPLAY_LINE_1], "          ");
  strcpy(display_line[DISPLAY_LINE_2], "  IR:ON   ");
  strcpy(display_line[DISPLAY_LINE_3], "          ");
  strcpy(display_line[DISPLAY_LINE_4], "PRESS SW1 ");
  lcd_4line();
  
// This line identifies the text on the Display is to change.
  display_changed = TRUE;
  
  motors_off();         //disable
  
  sec_flag = ON;
  TB0CCTL1 |=  CCIE;    //enables 1 second wait interrupt timer to space out initializations
//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
    if(onetime == ON && sec_flag == OFF){  //check if second has elapsed, perform once
      Init_ADC();                          //initialize ADC
      IOT_AUTO_active_flag = AUTO;         //set flag to AUTO, begin movement state for calibration
      onetime = OFF;                       //set onetime active flag to low
    }
    
    if(IOT_AUTO_active_flag == IOT){    //if in IOT active state, send IOT automatic commands
      iot_startup();
    }
    if(IOT_AUTO_active_flag == AUTO){   //if in AUTO active state, handle calibration/blackline processes
      movement();
    }

    //UCA0 FLAGS ===============================================================
    if((UCA0_read_flag == ON) && (iot_rx_wr != usb_rx_rd)){     //if read flag high and not up to date
      UCA0_proc_buff[UCA0_proc_index] = IOT_2_PC[usb_rx_rd];    //read into process buffer
      IOT_2_PC[usb_rx_rd] = ASCII_null;                         //clear RX ring buffer at index read
      if(UCA0_proc_buff[UCA0_proc_index] == ASCII_newline){     //last char detected ('\n')
        UCA0_proc_index = BEGINNING;                            //set index to start
        
        UCA0_process_flag = ON;                                 //set flag high to process message received
        UCA0_read_flag = OFF;                                   //set read flag low until next RX transmission
      }
      else{
        UCA0_proc_index++;                      //message not done, increment index
      }
      usb_rx_rd++;                              //increment read counter
      if(usb_rx_rd >= (sizeof(IOT_2_PC))){      //if read counter is greater than ring buffer
        usb_rx_rd = BEGINNING;                  //set index back to start
        UCA0_read_flag = OFF;                   //set read flag low, wait for next character transmission to be received (paces the program to not read too fast)
      }
    }
    
    if(UCA0_process_flag == ON){        
      UCA0_process_flag = OFF;          //set flag low
      iot_message_process();            //call command to process message in process buffer
      UCA0_process_flag = OFF;          //set flag low
    }
    
    if(UCA0_transmit_flag == ON){
      for(int i = BEGINNING; i < DBL_RING_SIZE; i++){
        usb_rx_wr++;                            //increment write counter
        PC_2_IOT[i] = ASCII_null;               //clear TX ring buffer of previous characters   
        
        PC_2_IOT[i] = proc_buff_hold[i];        //copy from process buffer to TX ring buffer
      }
      UCA0IE |= UCTXIE;                         //enable TX interrupt
      UCA0_transmit_flag = OFF;                 //set transmit flag low
    }  
  }
//------------------------------------------------------------------------------
}




