//-------------------------------------------------------------------------------
//
//  File Name: led.c
//
//  Description: This file contains the FRAM Board LED/LCD commands handling
//               display updates, disabling LEDs, converting and displaying ADC
//               samples, and clearing the LCD display
//
//  Alex Daum
//  Aug 2022
//  Built with IAR Embedded Workbench Version: V7.21.1
//-------------------------------------------------------------------------------
//  Globals - none
//-------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

extern volatile unsigned char display_changed; //CHANGED TO EXTERN
extern volatile unsigned char update_display;
extern volatile unsigned int Time_Sequence; //CHANGED TO EXTERN
extern volatile char one_time; //CHANGED TO EXTERN

extern char display_line[DISPLAY_LINE_TOTAL][11];
unsigned char adc_char[ADC_CHAR_LIMIT];

//==============================================================================
// Function Name: Display_Process
//
// Description: Handles LCD display updates when update_display and 
//              display_changed are true
//
// Date: Aug 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Display_Process(void){
  if(update_display){                                                   //set high every 200ms
    update_display = RESET_STATE;                                       //set flag low
    if(display_changed){                                                //check if display ready to update
      display_changed = RESET_STATE;                                    //set flag low
      Display_Update(RESET_STATE,RESET_STATE,RESET_STATE,RESET_STATE);  //process display changes
    }
  }
}

//==============================================================================
// Function Name: Init_LEDs
//
// Description: Disables LEDs
//
// Date: Aug 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns off both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}

//==============================================================================
// Function Name: HextoBCD
//
// Description: Converts given hex value to a BCD value to display on LCD screen
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void HEXtoBCD(int hex_value){
  int value = RESET_STATE;                              //reset holder variable
  for(int i=BEGINNING; i < ADC_CHAR_LIMIT; i++) {       //loop for all 4 positions of ADC value
    adc_char[i] = ASCII_0;                              //clear ADC values
  }
  while (hex_value > THOUSANDS_LIMIT){                  //check if less than 1000
    hex_value = hex_value - THOUSANDS_DECREMENT;        //subtract 1000 from value
    value = value + ADC_VAL_INCREMENT;                  //increment holder variable
    adc_char[ADC_CHAR_THOUSANDS] = HEX_0 + value;       //convert and add to thousands position of ADC char array
  }
  value = RESET_STATE;                                  //clear holder variable
  while (hex_value > HUNDREDS_LIMIT){                   //check if less than 100
    hex_value = hex_value - HUNDREDS_DECREMENT;         //subtract 100 from value
    value = value + ADC_VAL_INCREMENT;                  //increment holder variable
    adc_char[ADC_CHAR_HUNDREDS] = HEX_0 + value;        //convert and add to hundreds position of ADC char array
  }
  value = RESET_STATE;                                  //clear holder variable
  while (hex_value > TENS_LIMIT){                       //check if less than 10
    hex_value = hex_value - TENS_DECREMENT;             //subtract 10 from value
    value = value + ADC_VAL_INCREMENT;                  //increment holder variable
    adc_char[ADC_CHAR_TENS] = HEX_0 + value;            //convert and add to tens position of ADC char array
  }
  adc_char[ADC_CHAR_ONES] = HEX_0 + hex_value;          //convert and add remaining value to ones position of ADC char array
}

//==============================================================================
// Function Name: adc_line
//
// Description: ADC line insert function - Inserts BCD value in array adc_char
//              in specific location on LCD display
//
//              char line - specifies line 1-4 on display
//              char location - specifies location 0-9 on display
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void adc_line(char line, char location){
  unsigned int real_line;
  real_line = line - LINE_DECREMENT;                    //adjust for referencing screen indexes
  for(int i=BEGINNING; i < ADC_CHAR_LIMIT; i++) {       //loop from thousands to ones place of ADC char array
    display_line[real_line][i+location] = adc_char[i];  //copy from ADC char array to screen
  }
  display_changed = TRUE;
}

//==============================================================================
// Function Name: clear_screen
//
// Description: Clears LCD screen, sets variable to update display
//
// Author: Alex Daum
// Date: Oct 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void clear_screen(void){
  strcpy(display_line[DISPLAY_LINE_1], "          ");
  strcpy(display_line[DISPLAY_LINE_2], "          ");   //clear all four display lines
  strcpy(display_line[DISPLAY_LINE_3], "          ");
  strcpy(display_line[DISPLAY_LINE_4], "          ");
  display_changed = TRUE;
}