//-------------------------------------------------------------------------------
//
//  File Name: init.c
//
//  Description: This file contains the the Initialization Process using the
//               function Init_Conditions(void)
//
//  Alex Daum
//  Sept 2022
//  Built with IAR Embedded Workbench Version: V7.21.1
//-------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"

extern char display_line[DISPLAY_LINE_TOTAL][SCREEN_LIMIT_END];
extern char *display[DISPLAY_LINE_TOTAL];
extern volatile unsigned char update_display;


void Init_Conditions(void){
//------------------------------------------------------------------------------
// Init_Condtitions Program
// This resets display variables to low and enables interrupts when called.
//
//  Globals used - char display_line[4][11], char *display[4], 
//                 volatile unsigned char update_display
//  Inputs - None, Outputs - None
//------------------------------------------------------------------------------

  int i;
  for(i=BEGINNING;i<SCREEN_LIMIT_END;i++){
    display_line[DISPLAY_LINE_1][i] = RESET_STATE;
    display_line[DISPLAY_LINE_2][i] = RESET_STATE;
    display_line[DISPLAY_LINE_3][i] = RESET_STATE;
    display_line[DISPLAY_LINE_4][i] = RESET_STATE;
  }
  display_line[DISPLAY_LINE_1][SCREEN_LIMIT] = RESET_STATE;
  display_line[DISPLAY_LINE_2][SCREEN_LIMIT] = RESET_STATE;
  display_line[DISPLAY_LINE_3][SCREEN_LIMIT] = RESET_STATE;
  display_line[DISPLAY_LINE_4][SCREEN_LIMIT] = RESET_STATE;

  display[DISPLAY_LINE_1] = &display_line[DISPLAY_LINE_1][BEGINNING];
  display[DISPLAY_LINE_2] = &display_line[DISPLAY_LINE_2][BEGINNING];
  display[DISPLAY_LINE_3] = &display_line[DISPLAY_LINE_3][BEGINNING];
  display[DISPLAY_LINE_4] = &display_line[DISPLAY_LINE_4][BEGINNING];
  update_display = RESET_STATE;


// Interrupts are disabled by default, enable them.
  enable_interrupts();
//------------------------------------------------------------------------------
}