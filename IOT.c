//------------------------------------------------------------------------------
//
//  File Name: IOT.c
//
//  Description: This file contains a state machine for IOT startup and
//               maintenance, also contains function for command processing
//
//  Alex Daum
//  Sept 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include "ports.h"
#include <string.h>

extern char display_line[DISPLAY_LINE_TOTAL][SCREEN_LIMIT_END];
extern volatile unsigned char display_changed;
extern volatile unsigned char timer_flag;

extern unsigned char UCA0_proc_buff[DBL_RING_SIZE];
extern char proc_buff_hold[DBL_RING_SIZE];
unsigned char cmd_flag;
extern volatile unsigned char next_cmd_flag;
extern volatile unsigned char UCA0_transmit_flag;
volatile unsigned char iot_state = IOT_STARTUP;

extern volatile char state;

unsigned char pad_on_flag = OFF;
unsigned char pad_num;
unsigned char stop_cmd_flag = OFF;
unsigned char movement_cmd_flag = OFF;
unsigned char auto_flag = OFF;
unsigned int cmd_duration;
volatile unsigned char move_cmd_ready = ON;

unsigned char SSID_flag = OFF;
unsigned char IP_flag = OFF;
unsigned char retrieve_flag = OFF;
unsigned int retrieve_index;
unsigned int IP_group_count;

extern unsigned char IOT_AUTO_active_flag;

//==============================================================================
// Function Name: iot_startup
//
// Description: Handles enabling and initialization of IOT module, afterwards
//              pings google at regular intervals
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void iot_startup(void){
  switch (iot_state){
    case IOT_STARTUP:
      TB1CCTL1 |= CCIE;         //enable IOT auto command wait interval interrupt
      
      if(next_cmd_flag == ON){  //if ready for next command
        P3OUT  |=  IOT_EN_CPU;  //set iot_en_cpu high
        next_cmd_flag = OFF;    //set flag low, wait interval for next command
        iot_state = SAVE_OFF;   //move to next state
      }
      break;
      
    case SAVE_OFF: //Disable Save Settings
      strncpy(proc_buff_hold, "AT+SYSSTORE=0\r\n", IOT_MSG_LENGTH);     //copy IOT command to process buffer
      if(next_cmd_flag == ON){                                          //if ready for next IOT command
        next_cmd_flag = OFF;                                            //set flag low, wait interval for next command
        UCA0_transmit_flag = ON;                                        //set transmit flag high, moves string from process buffer to TX buffer
        iot_state = HARD_RESET;                                         //move to next state
      }
      break;
      
    case HARD_RESET: //Hard Reset
      strncpy(proc_buff_hold, "AT+CIPMUX=1\r\n", IOT_MSG_LENGTH);       //copy IOT command to process buffer
      if(next_cmd_flag == ON){                                          //if ready for next IOT command
        next_cmd_flag = OFF;                                            //set flag low, wait interval for next command    
        UCA0_transmit_flag = ON;                                        //set transmit flag high, moves string from process buffer to TX buffer
        iot_state = SOFT_RESET;                                         //move to next state
      }
      break;
      
    case SOFT_RESET: //Soft Reset
      strncpy(proc_buff_hold, "AT+CIPSERVER=1,3547\r\n", IOT_MSG_LENGTH);       //copy IOT command to process buffer
      if(next_cmd_flag == ON){                                                  //if ready for next IOT command
        next_cmd_flag = OFF;                                                    //set flag low, wait interval for next command 
        UCA0_transmit_flag = ON;                                                //set transmit flag high, moves string from process buffer to TX buffer
        iot_state = IP_REQ;                                                     //move to next state
      }
      break;
      
    case IP_REQ: //IP Request
      strncpy(proc_buff_hold, "AT+CIFSR\r\n", IOT_MSG_LENGTH);          //copy IOT command to process buffer
      if(next_cmd_flag == ON){                                          //if ready for next IOT command
        next_cmd_flag = OFF;                                            //set flag low, wait interval for next command
        UCA0_transmit_flag = ON;                                        //set transmit flag high, moves string from process buffer to TX buffer
        P1OUT |= RED_LED;                                               //turn on red led to indicate transmission
        iot_state = PING;                                               //move to next state
      }
      break;
      
    case PING:
      strncpy(proc_buff_hold, "AT+PING=\"GOOGLE.COM\"\r\n", IOT_MSG_LENGTH);    //copy IOT command to process buffer
      if(next_cmd_flag == ON){                                                  //if ready for next IOT command
        next_cmd_flag = OFF;                                                    //set flag low, wait interval for next command
        UCA0_transmit_flag = ON;                                                //set transmit flag high, moves string from process buffer to TX buffer
        iot_state = PING;                                                       //continue to ping
      }
      break;
    default: break;
  }
}

//==============================================================================
// Function Name: iot_message_process
//
// Description: Handles message processing in process buffer, handles IP 
//              retrieval and command processing 
//              Checks if command character is present with passcode, then sets
//              flags for commands present, flags are handled after the process
//              buffer has been read through and cleared
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void iot_message_process(void){
  for(int i = BEGINNING; i < DBL_RING_SIZE; i++){
    
    //COMMAND CHECK - check for '^' and 4 digit password
    if((UCA0_proc_buff[i] == CMD_CHAR) && (UCA0_proc_buff[i+PASS_1_OFFSET] == PASS_1_NUM) && (UCA0_proc_buff[i+PASS_2_OFFSET] == PASS_2_NUM) && (UCA0_proc_buff[i+PASS_3_OFFSET] == PASS_3_NUM) && (UCA0_proc_buff[i+PASS_4_OFFSET] == PASS_4_NUM)){
      if(UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_p){ //if command char is p
        pad_on_flag = ON;                               //set pad flag high
        pad_num = UCA0_proc_buff[i+CMD_PAD_NUM_OFFSET]; //record pad number
      }
      if(UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_o){ //if command char is o
        P6OUT &= ~LCD_BACKLITE;                         //disable backlite
      }
      if(UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_q){ //if command char is q
        stop_cmd_flag = ON;                             //set stop flag high
      }
      
      //movement commands 'w' 's' 'a' 'd'
      if(UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_w || UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_s || UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_a || UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_d){
        cmd_duration = RESET_STATE;                                                     //reset duration value
        cmd_duration += (UCA0_proc_buff[i+DURATION_10_OFFSET]-ASCII_0) * TENS_CONV;     //copy tens value of duration into variable
        cmd_duration += (UCA0_proc_buff[i+DURATION_1_OFFSET]-ASCII_0);                  //copy ones value of duration into variable
        movement_cmd_flag = UCA0_proc_buff[i+CMD_CHAR_OFFSET];                          //record what the command char was
      }
      if(UCA0_proc_buff[i+CMD_CHAR_OFFSET] == ASCII_f){ //if command char is f
        auto_flag = ON;                                 //set black line program flag high
      }
      
      timer_flag = ON;  //command received, start timer if not already on
    }
    
    //IP CHECK
    if(UCA0_proc_buff[i] == ASCII_plus){
      if((UCA0_proc_buff[i+IP_CHECK_INDEX_1] == ASCII_I) && (UCA0_proc_buff[i+IP_CHECK_INDEX_2] == ASCII_P)){   //if process buffer matches IP received format
        IP_flag = ON;                                                                                           //set IP retrieval flag high
      }
    }
    
    //IP RETRIEVAL
    if(IP_flag == ON){                 
      if(UCA0_proc_buff[i] == ASCII_quote){     //read until quote reached
        if(retrieve_flag == OFF){               //if first quotation mark
          retrieve_flag = ON;                   //start retrieving characters
          retrieve_index = RESET_STATE;         //reset index counter to 0
        }
        else{
          P1OUT &= ~RED_LED;                    //disable red LED to indicate IP retrieval complete
          strcpy(display_line[DISPLAY_LINE_1], "Waiting   ");
          strcpy(display_line[DISPLAY_LINE_2], "For Input:");
          
          display_changed = TRUE;           
          retrieve_flag = OFF;                  //second quotation mark reached, stop retrieving
          IP_flag = OFF;                        //set IP retrieval flag low
        }
      }
      else if(retrieve_flag == ON){             //while retrieval flag is high
        if(UCA0_proc_buff[i] == ASCII_period){  //check if '.' has been encountered
          if(IP_group_count++ == IP_GROUP_1){   //reset retrieval index for 2nd line
            retrieve_index = RESET_STATE;
          }
        }
        if(IP_group_count < IP_GROUP_2){                                        //if in first 2 groups of IP sequence
          display_line[DISPLAY_LINE_3][retrieve_index++] = UCA0_proc_buff[i];   //display to line 3
        }
        else{                                                                   //if in last 2 groups of IP sequence
          display_line[DISPLAY_LINE_4][retrieve_index++] = UCA0_proc_buff[i];   //display to line 4
        }
      }
    }
    
    proc_buff_hold[i] = UCA0_proc_buff[i];      //copy from process buffer into holder array
    UCA0_proc_buff[i] = ASCII_null;             //clear process buffer index on each iteration
  }
  
  if(pad_on_flag == ON){
    P6OUT |= LCD_BACKLITE;                                              //enable backlite
    strcpy(display_line[DISPLAY_LINE_1], "Arrived 0X");                 //display arrived text
    display_line[DISPLAY_LINE_1][PAD_NUM_DISPLAY_INDEX] = pad_num;      //indicate specific pad number
    strcpy(display_line[DISPLAY_LINE_2], "   ALEX   ");                 //display name
    strcpy(display_line[DISPLAY_LINE_3], "   DAUM   ");                 //
    display_changed = TRUE;
    pad_on_flag = OFF;                                                  //disable flag
  }
  if(stop_cmd_flag == ON){
    motors_off();                       //disable motors
    cmd_duration = RESET_STATE;         //set duration variable to 0, immediately finishes interrupt counter
    if(state == LINE_FOLLOW){           //if in line following movement state
      state = END_PROGRAM_START;        //trigger end program to leave circle
      P1OUT |= RED_LED;
    }
    strcpy(display_line[DISPLAY_LINE_4], "STOP  ");     //display stop text to screen
    display_changed = TRUE;
    stop_cmd_flag = OFF;                                //disable stop flag
  }
  if(movement_cmd_flag != OFF){         //check if any char except for OFF (w,a,s,d)
    if(move_cmd_ready == ON){           //check if movement command currently in progress
      motors_off();                     //disable motors
      strcpy(display_line[DISPLAY_LINE_4], "      ");
      display_line[DISPLAY_LINE_4][DURATION_10_DISPLAY_INDEX] = cmd_duration/TENS_CONV + ASCII_0;       //display tens position of duration value to screen
      display_line[DISPLAY_LINE_4][DURATION_1_DISPLAY_INDEX] = cmd_duration%TENS_CONV + ASCII_0;        //display ones position of duration value to screen
      switch(movement_cmd_flag){
        case ASCII_w:
          motor_forward();                                                      //if 'w' command, set motors forward
          display_line[DISPLAY_LINE_4][CMD_CHAR_DISPLAY_INDEX] = ASCII_w;       //display 'w' command to screen
          break;
        case ASCII_s:
          motor_reverse();                                                      //if 's' command, set motors reverse
          display_line[DISPLAY_LINE_4][CMD_CHAR_DISPLAY_INDEX] = ASCII_s;       //display 's' command to screen
          break;
        case ASCII_a:
          motor_turn_left();                                                    //if 'a' command, set motors to turn car left
          display_line[DISPLAY_LINE_4][CMD_CHAR_DISPLAY_INDEX] = ASCII_a;       //display 'a' command to screen
          break;
        case ASCII_d:
          motor_turn_right();                                                   //if 'd' command, set motors to turn car right
          display_line[DISPLAY_LINE_4][CMD_CHAR_DISPLAY_INDEX] = ASCII_d;       //display 'd' command to screen
          break;
      }
      display_changed = TRUE;
      move_cmd_ready = OFF;             //set flag low, not ready for next movement command
      movement_cmd_flag = OFF;          //set flag low, movement command processed
      TB1CCTL2 |= CCIE;                 //enable timer interrupt for duration timer
    }
  }
  if(auto_flag == ON){
    IOT_AUTO_active_flag = AUTO;                        //set char for black line program
    strcpy(display_line[DISPLAY_LINE_4], "AUTO  ");     //display command on screen
    display_changed = TRUE;                             
    state = BLACK_LINE_START;                           //change movement state to black line start state
    auto_flag = OFF;                                    //set flag low
  }
}

