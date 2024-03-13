//-------------------------------------------------------------------------------
//  
//  File Name: interrupt.c
//
//  Description: This file contains all interrupts used, hardware interrupts 
//               (sw1 & sw2), timer interrupts, adc interrupts, and serial
//               interrupts
//
//  Alex Daum
//  Oct 2022
//  Built with IAR Embedded Workbench Version: V7.21.1
//-------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include "ports.h"
#include <string.h>

extern char display_line[DISPLAY_LINE_TOTAL][SCREEN_LIMIT_END];
extern volatile unsigned char display_changed;

extern volatile unsigned char update_display;

volatile unsigned int debounce_count_sw1;
volatile unsigned int debounce_count_sw2;
volatile char sw1_debounce = OFF;
volatile char sw2_debounce = OFF;

volatile unsigned int Global_ADC_Channel;
volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Left_Detect;
volatile unsigned int ADC_Right_Detect;
volatile unsigned int ADC_Thumb_Detect;
extern volatile unsigned char adc_char[4];

volatile unsigned int Left_Detect_Val;         
volatile unsigned int Right_Detect_Val;
volatile unsigned int Left_Detect_Hold;
volatile unsigned int Right_Detect_Hold;

extern volatile char state;
volatile unsigned int sec_interrupt_count;
volatile char sec_flag = OFF;
volatile unsigned int sec_5_interrupt_count;
volatile char sec_5_flag = OFF;
volatile unsigned int circle_stop_count;

// UCA0 ========================================================================
volatile unsigned char UCA0_read_flag = OFF;
volatile unsigned char UCA0_process_flag = OFF;
volatile unsigned char UCA0_transmit_flag = OFF;
volatile unsigned int UCA0_temp;


volatile unsigned char next_cmd_flag = OFF;
volatile unsigned int next_cmd_count;
extern volatile unsigned char iot_state;
volatile unsigned char end_UCA0_TX_flag = OFF;
extern unsigned int cmd_duration;
volatile unsigned int cmd_duration_count;
extern volatile unsigned char move_cmd_ready;

//Serial Variables
extern volatile unsigned char PC_2_IOT[SMALL_RING_SIZE];
extern volatile unsigned int usb_rx_wr;
extern unsigned int direct_usb;
extern volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
extern volatile unsigned int iot_rx_wr;  
extern unsigned int direct_iot;

//On-screen Timer
volatile unsigned char timer_flag = OFF;
volatile unsigned int timer_count;
volatile unsigned int timer_count_hold;

//Blackline
volatile unsigned int right_BL_target;
volatile unsigned int left_BL_target;
volatile unsigned int right_white_target;
volatile unsigned int left_white_target;

//==============================================================================
//HARDWARE INTERRUPTS ----------------------------------------------------------
//==============================================================================

//==============================================================================
// Interrupt: Switch 1
//
// Description: Handles switch 1 actions and triggers debounce timer
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
#pragma vector = PORT4_VECTOR
__interrupt void switchP4_interrupt(void){      //USED FOR STARTING MOVEMENT
  //SW1
  P4IFG &= ~SW1;        //sets switch press flag to 0
  P4IE  &= ~SW1;        //disables sw1 press interrupt
  
  if(state == CONFIG_HIGH){                                     //check if movement state machine is in CONFIG_HIGH state
    right_BL_target = Left_Detect_Val - RIGHT_DETECT_ADJUSTMENT;//adjust detect threshold to allow greater margin of error, swap right/left to match motor right/left       
    left_BL_target = Right_Detect_Val - LEFT_DETECT_ADJUSTMENT; //adjust detect threshold to allow greater margin of error, swap right/left to match motor right/left
  }                                                             
  else if(state == CONFIG_LOW){                                 //check if state machine is in CONFIG_HIGH state
    right_white_target = Left_Detect_Val;                       //swap right/left to match motor right/left
    left_white_target = Right_Detect_Val;                       //swap right/left to match motor right/left
  }
  
  sw1_debounce = ON;                    //signals switch in debounce
  debounce_count_sw1 = RESET_STATE;     //reset debounce counter
  TB0CCTL1 |=  CCIE;                    //enables interrupt timer
}

//==============================================================================
// Interrupt: Switch 2
//
// Description: Handles switch 2 actions and triggers debounce timer
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
#pragma vector = PORT2_VECTOR
__interrupt void switchP2_interrupt(void){ //USED FOR ENABLING/DISABLING IR EMITTER
  //SW2
  P2IFG &= ~SW2;        //sets switch press flag to 0
  P2IE  &= ~SW2;        //disables sw2 press interrupt interrupt
  
  sw2_debounce = ON;                    //signals switch in debounce
  debounce_count_sw2 = RESET_STATE;     //reset debounce counter
  TB0CCTL2 |=  CCIE;                    //enables interrupt timer
}

//==============================================================================
//TIMER INTERRUPTS -------------------------------------------------------------
//==============================================================================

//==============================================================================
// Interrupt: Timer 0 Interrupts
//
// Description: Handles timer 0 interrupt actions
//              B0 - updating on-screen timer and LCD screen updates
//              B1 - second interval timer, BL line configuration + sw1 debounce
//              B2 - Switch debounce
//              Overflow - unused
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
#pragma vector = TIMER0_B0_VECTOR                      //HANDLES UPDATING SCREEN
__interrupt void Timer0_B0_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB0 0 Interrupt handler
  //----------------------------------------------------------------------------        
            //add what happens in interrupt
  
  if(timer_flag == ON){                         //set timer_flag high to start on-screen count
    timer_count += SCREEN_TIMER_INTERVAL;       //increase counter by 200ms
    timer_count_hold = timer_count/TENS_CONV;   //remove tenths place
    
    display_line[DISPLAY_LINE_4][SCREEN_TIMER_INDEX_4] = ASCII_s;                               //add 's' to end of screen count
    display_line[DISPLAY_LINE_4][SCREEN_TIMER_INDEX_3] = timer_count_hold%TENS_CONV + ASCII_0;  //display ones place
    timer_count_hold /= TENS_CONV;                                                              //shift to tens place
    display_line[DISPLAY_LINE_4][SCREEN_TIMER_INDEX_2] = timer_count_hold%TENS_CONV + ASCII_0;  //display tens place
    timer_count_hold /= TENS_CONV;                                                              //shift to hundreds place
    display_line[DISPLAY_LINE_4][SCREEN_TIMER_INDEX_1] = timer_count_hold%TENS_CONV + ASCII_0;  //display hundreds place
    timer_count_hold /= TENS_CONV;
    
    display_changed = TRUE;
  }
  
  update_display = TRUE;                // set high to check if screen should update every 200ms
  TB0CCR0 += TB0CCR0_INTERVAL;          // Add Offset to TBCCR0
  //----------------------------------------------------------------------------
}

#pragma vector = TIMER0_B1_VECTOR                       //HANDLES SWITCH DEBOUNCE AND MOVE TIMER
__interrupt void TIMER0_B1_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
  //----------------------------------------------------------------------------
  switch(__even_in_range(TB0IV,OVERFLOW)){
    case NO_INTERRUPT: break;                              // No interrupt
    case CCR1:                                     // CCR1 - Wait 1sec to start car  (REFIT TO HALT BETWEEN FORWARD/BACKWARD
      //add interrupt actions
      if(sec_interrupt_count++ >= SEC_TIMER_INTERRUPT){         //increment counter until 1 sec reached
        sec_interrupt_count = RESET_STATE;                      //reset counter
        TB0CCTL1 &= ~CCIE;                                      //disable Wait Timer
        if(sec_flag != ON){                     //no flag set indicates button wait
          if(state == IDLE){                    //if movement state machine in idle, 
            state = CONFIG_HIGH;                //  go to config high vals state
          }
          else if(state == CONFIG_HIGH){        //if movement state machine in config high state,
            state = CONFIG_LOW;                 //  go to config low vals state
          }
          else if(state == CONFIG_LOW){         //if movement state machine in config low state,
            state = CONFIG_DONE;                //  go to config finished state
          }
          else if(state == CONFIG_DONE){        //if movement state machine in config finished state,
            state = IOT_ACTIVE;                 //  go to IOT active state
          }
          P4IE  |=  SW1;                        // Reenable Switch Interrupt
        }
        else{                                   //sec flag set indicates software wait
          sec_flag = OFF;                       //sec count reached, set flag low
        }
      }
      TB0CCR1 += TB0CCR1_INTERVAL;              // Add Offset to TBCCR1
      break;
    case CCR2:                                  // CCR2 - Switch debounce timer
      //add interrupt actions
      if(sw1_debounce == ON){
        debounce_count_sw1++;                           //increment debounce counter
        if(debounce_count_sw1 >= SW_TIMER_INTERRUPT){   //check if counter reaches debounce wait limit
          P4IE |=  SW1;                                 //enable switch 1 interrupt
          sw1_debounce = OFF;                           //indicate debounce is done
        }
      }
      if(sw2_debounce == ON){
        debounce_count_sw2++;                           //increment debounce counter
        if(debounce_count_sw2 >= SW_TIMER_INTERRUPT){   //check if counter reaches debounce wait limit
          P2IE |=  SW2;                                 //enable switch 2 interrupt
          sw2_debounce = OFF;                           //indicate debounce is done
        }
      }
      if((sw1_debounce == OFF) && (sw2_debounce == OFF)){//if no switches are currently being debounced
        TB0CCTL2 &= ~CCIE;                               //disable debounce timer
      }
      TB0CCR2 += TB0CCR2_INTERVAL;              //Add Offset to TBCCR2
      break;
    case OVERFLOW:
      //add interrupt actions
      break;
    default: break;
  }
}

//==============================================================================
// Interrupt: Timer 1 Interrupts
//
// Description: Handles timer 1 interrupt actions
//              B0 - ADC sampling interval timer
//              B1 - IOT automatic message transmission interval
//              B2 - IOT movement command duration timer
//              Overflow - unused
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
#pragma vector = TIMER1_B0_VECTOR                      
__interrupt void TIMER1_B0_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB1 0 Interrupt handler
  //----------------------------------------------------------------------------        
            //add what happens in interrupt
  //ENABLE NEXT ADC SAMPLE
  ADCCTL0 |= ADCENC;                     //enable Conversions
  ADCCTL0 |= ADCSC;                      //start next sample
  
  Right_Detect_Hold = Left_Detect_Val;   //use Samples Taken
  Left_Detect_Hold = Right_Detect_Val;   //swap right/left to match motor right/left
  
  TB1CCR0 += TB1CCR0_INTERVAL;          // Add Offset to TBCCR0
  //----------------------------------------------------------------------------
}

#pragma vector = TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB1 1-2, Overflow Interrupt Vector (TBIV) handler
  //----------------------------------------------------------------------------
  switch(__even_in_range(TB1IV,OVERFLOW)){
    case NO_INTERRUPT: break;                              // No interrupt
    case CCR1:                                             // CCR1 -- IOT AUTO MESSAGE INTERVAL TIMER
      //add interrupt actions
      next_cmd_count++;                                    //increment counter
      
      if(iot_state == PING){                               //if iot state machine in ping state
        if(next_cmd_count >= NEXT_CMD_PING){               //wait for ping interval
          next_cmd_count = RESET_STATE;                    //reset counter
          next_cmd_flag = ON;                              //ready for next ping transmission
        }
      }
      else if(next_cmd_count >= NEXT_CMD_COUNT_LIMIT){     //if counter is past next command wait limit
        next_cmd_count = RESET_STATE;                      //reset counter
        next_cmd_flag = ON;                                //ready for next automatic iot transmission
      }
      
      TB1CCR1 += TB1CCR1_INTERVAL;              // Add Offset to TBCCR1
      break;
    case CCR2:                                                          // CCR2 -- WHEEL COMMAND DURATION
      //add interrupt actions
      if(cmd_duration_count++ >= (cmd_duration+DURATION_ADJUSTMENT)){   //increment and check if counter has reached command duration
        motors_off();                                                   //disable motors once duration reached
      }
      if(cmd_duration_count++ >= (cmd_duration+DURATION_WAIT_ADJUSTMENT)){      //sit idle for interval to prevent motor damage
        motors_off();                                                           //disable motors in case triggered by stop command
        move_cmd_ready = ON;                                                    //indicate ready for next movement command
        cmd_duration_count = RESET_STATE;                                       //reset counter
        TB1CCTL2 &= ~CCIE;                                                      //disable timer interrupt
      }
      
      TB1CCR2 += TB1CCR2_INTERVAL;              //Add Offset to TBCCR2
      break;
    case OVERFLOW:
      //add interrupt actions
      break;
    default: break;
  }
}

//==============================================================================
// Interrupt: Timer 2 Interrupts
//
// Description: Handles timer 2 interrupt actions
//              B0 - unused
//              B1 - 5 second interval timer
//              B2 - BL program circle wait timer
//              Overflow - unused
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
#pragma vector = TIMER2_B0_VECTOR                      //
__interrupt void TIMER2_B0_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB2 0 Interrupt handler
  //----------------------------------------------------------------------------        
            //add what happens in interrupt
  TB2CCR0 += TB2CCR0_INTERVAL;          // Add Offset to TBCCR0
  //----------------------------------------------------------------------------
}

#pragma vector = TIMER2_B1_VECTOR
__interrupt void TIMER2_B1_ISR(void){
  //----------------------------------------------------------------------------
  // TimerB2 1-2, Overflow Interrupt Vector (TBIV) handler
  //----------------------------------------------------------------------------
  switch(__even_in_range(TB2IV,OVERFLOW)){
    case NO_INTERRUPT: break;                                   // No interrupt
    case CCR1:                                                  // CCR1 -- 5 SECOND WAIT TIMER
      //add interrupt actions
      
      if(sec_5_interrupt_count++ >= SEC_5_TIMER_INTERRUPT){     //check if counter has reached 5 sec interval
        sec_5_interrupt_count = RESET_STATE;                    //reset counter
        TB2CCTL1 &= ~CCIE;                                      //disable 5 sec interrupt
        sec_5_flag = OFF;                                       //set flag low to indicate 5 secs complete
      }
      
      TB2CCR1 += TB2CCR1_INTERVAL;                              // Add Offset to TBCCR1
      break;
    case CCR2:                                                  // CCR2 -- TIMER TO STOP CAR IN CIRCLE
      //add interrupt actions
      
      if(circle_stop_count++ >= CIRCLE_STOP_TIME){              //if counter has reached circle time
        motors_off();                                           //disable motors
        state = CIRCLE_WAIT;                                    //set movement state to circle wait to prevent line following
        P6OUT |= LCD_BACKLITE;                                  //enable backlite
        strcpy(display_line[DISPLAY_LINE_1], "BL CIRCLE ");     //display circle wait text
        display_changed = TRUE;
        
        if(sec_5_flag == OFF){                                  //check if 5 seconds has elapsed
          P6OUT &= ~LCD_BACKLITE;                               //disable backlite
          state = LINE_FOLLOW;                                  //change movement state back to line following
          TB2CCTL2 &= ~CCIE;                                    //disable circle wait interrupt
        }
        else{
          TB2CCTL1 |=  CCIE;                                    //enables 5 sec wait timer to stop and display text
        }
      }
      
      TB2CCR2 += TB2CCR2_INTERVAL;              //Add Offset to TBCCR2
      break;
    case OVERFLOW:
      //add interrupt actions
      break;
    default: break;
  }
}

//==============================================================================
// ADC INTERRUPT ===============================================================
//==============================================================================
//==============================================================================
// Interrupt: ADC
//
// Description: Handles ADC sampling, includes function to display IR detector
//              readings to the screen
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
  case ADCIV_NONE:
    break;
  case ADCIV_ADCOVIFG:                  // When a conversion result is written to the ADCMEM0
                                        // before its previous conversion result was read.
    break;
  case ADCIV_ADCTOVIFG:                 // ADC conversion-time overflow
    break;
  case ADCIV_ADCHIIFG:                  // Window comparator interrupt flags
    break;
  case ADCIV_ADCLOIFG:                  // Window comparator interrupt flag
    break;
  case ADCIV_ADCINIFG:                  // Window comparator interrupt flag
    break;
  case ADCIV_ADCIFG:                    // ADCMEM0 memory register with the conversion result
    ADCCTL0 &= ~ADCENC;                 // Disable ENC bit.
    switch (ADC_Channel++){
      case LEFT_DETECTOR_CHANNEL:                               // Channel A2 Interrupt
        ADCMCTL0 &= ~ADCINCH_2;                                 // Disable Last channel A2
        ADCMCTL0 |= ADCINCH_3;                                  // Enable Next channel A3
        ADC_Left_Detect = ADCMEM0;                              // Move result into Global
        ADC_Left_Detect = ADC_Left_Detect >> DIVIDE_BY_4;       // Divide the result by 4
        
        Left_Detect_Val = ADC_Left_Detect >> DIVIDE_BY_16;      // Divide by 16
        
        if(state == IDLE || state == CONFIG_HIGH || state == CONFIG_LOW){
          HEXtoBCD(Left_Detect_Val);                            // Convert result to String
          adc_line(DETECT_DISPLAY_LINE,DETECT_DISPLAY_LEFT);    // Place String in Display        
        }
        break;
      case RIGHT_DETECTOR_CHANNEL:        
        ADCMCTL0 &= ~ADCINCH_3;                                 // Disable Last channel A3
        ADCMCTL0 |= ADCINCH_5;                                  // Enable Next channel A5
        ADC_Right_Detect = ADCMEM0;                             // Move result into Global
        ADC_Right_Detect = ADC_Right_Detect >> DIVIDE_BY_4;     // Divide the result by 4
        
        Right_Detect_Val = ADC_Right_Detect >> DIVIDE_BY_16;    // Divide by 16
        
        if(state == IDLE || state == CONFIG_HIGH || state == CONFIG_LOW){
          HEXtoBCD(Right_Detect_Val);                           // Convert result to String
          adc_line(DETECT_DISPLAY_LINE,DETECT_DISPLAY_RIGHT);   // Place String in Display
        }
        break;
      case THUMB_WHEEL_CHANNEL:
        ADCMCTL0 &= ~ADCINCH_5;                                 // Disable Last channel A5
        ADCMCTL0 |= ADCINCH_2;                                  // Enable Next channel A2
        ADC_Thumb_Detect = ADCMEM0;                             // Move result into Global
        ADC_Channel=RESET_STATE;
        
        break;
      default:
        break;
    }
  default:
    break;
  }
}


//==============================================================================
// SERIAL INTERRUPTS ===========================================================
//==============================================================================
//==============================================================================
// Interrupt: Serial UCA0 RX/TX
//
// Description: Handles UCA0 receive and transmit interrupts
//
// Author: Alex Daum
// Date: Nov 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
//-----------------------------------------------------------------------------
// Echo back RXed character, confirm TX buffer is ready first
  switch(__even_in_range(UCA0IV,UCA0_RANGE)){
    case NO_INTERRUPT: break;                                // Vector 0 - no interrupt

    case RECEIVE_INTERRUPT:                                  // Vector 2 - RXIFG
      if (iot_rx_wr >= (sizeof(IOT_2_PC))){
        iot_rx_wr = BEGINNING;                               // Circular buffer back to beginning
      }
      UCA0_temp = iot_rx_wr++;                               // Increment temp index
      IOT_2_PC[UCA0_temp] = UCA0RXBUF;                       // Move value from RX buffer to RX ring buffer
      UCA0_read_flag = ON;                                   // Set flag high, indicates something to read from ring buffer
      break;
  
    case TRANSMIT_INTERRUPT:                                 // Vector 4 - TXIFG
      UCA0TXBUF = PC_2_IOT[direct_iot++];                    // Move from TX ring buffer to TX buffer
      if (direct_iot >= (sizeof(PC_2_IOT))){                 // Check if index larger than ring buffer
        direct_iot = BEGINNING;                              // Set index back to zero
      }
      if(end_UCA0_TX_flag == ON){                            // If newline detected on previous iteration
        end_UCA0_TX_flag = OFF;                              // Set flag low
        UCA0IE &= ~UCTXIE;                                   // Disable TX interrupt           
        direct_iot = BEGINNING;                              // Set index back to zero
        usb_rx_wr = BEGINNING;                               // Set write index to zero
      }
      if(PC_2_IOT[direct_iot] == ASCII_newline){             // If newline character read       
        end_UCA0_TX_flag = ON;                               // Set flag high, allows newline to be written to TX buffer in before stopping
      }
      break;
    
    default: break;
  }
//------------------------------------------------------------------------------
}