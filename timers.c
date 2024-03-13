//------------------------------------------------------------------------------
//
//  File Name: timers.c
//
//  Description: This file contains the initialization of timers 0-3, timers 0-2
//               are used for timer interrupts, timer 3 is used for PWM wheel
//               control
//
//  Alex Daum
//  Sept 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include "ports.h"

//==============================================================================
// Function Name: Init_Timers
//
// Description: Calls initialization functions for timers 0-3
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Init_Timers(void){
  Init_Timer_B0();
  Init_Timer_B1();
  Init_Timer_B2();
  Init_Timer_B3();
}

//==============================================================================
// Function Name: Init_Timer_B0
//
// Description: Initializes/configures timer 0 and timer 0 interrupts
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Init_Timer_B0(void){
  TB0CTL = TBSSEL__SMCLK;               // SMCLK source
  TB0CTL |= TBCLR;                      // resets TB0R, clock divider, count direction
  TB0CTL |= MC__CONTINOUS;              // continuous up
  TB0CTL |= ID__4;                      // divide clock by 4
  
  TB0EX0 = TBIDEX__8;                   // divide clock by an additional 8
  
  TB0CCR0 = TB0CCR0_INTERVAL;           // CCR0
  TB0CCTL0 |= CCIE;                     // CCR0 enable interrupt
  
  TB0CCR1 = TB0CCR1_INTERVAL;           // CCR1
  //TB0CCTL1 |= CCIE;                     // CCR1 enable interrupt
  
  TB0CCR2 = TB0CCR2_INTERVAL;           // CCR2
  //TB0CCTL2 |= CCIE;                   // CCR2 enable interrupt
  
  TB0CTL &= ~TBIE;                      // disable overflow interrupt
  TB0CTL &= ~TBIFG;                     // clear overflow interrupt flag 
}

//==============================================================================
// Function Name: Init_Timer_B1
//
// Description: Initializes/configures timer 1 and timer 1 interrupts
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Init_Timer_B1(void){
  TB1CTL = TBSSEL__SMCLK;               // SMCLK source
  TB1CTL |= TBCLR;                      // resets TB0R, clock divider, count direction
  TB1CTL |= MC__CONTINOUS;              // continuous up
  TB1CTL |= ID__8;                      // divide clock by 4
  
  TB1EX0 = TBIDEX__8;                   // divide clock by an additional 8
  
  TB1CCR0 = TB1CCR0_INTERVAL;           // CCR0
  //TB1CCTL0 |= CCIE;                     // CCR0 enable interrupt
  
  TB1CCR1 = TB1CCR1_INTERVAL;           // CCR1
  //TB1CCTL1 |= CCIE;                     // CCR1 enable interrupt
  
  TB1CCR2 = TB1CCR2_INTERVAL;           // CCR2
  //TB1CCTL2 |= CCIE;                   // CCR2 enable interrupt
  
  TB1CTL &= ~TBIE;                      // disable overflow interrupt
  TB1CTL &= ~TBIFG;                     // clear overflow interrupt flag 
}

//==============================================================================
// Function Name: Init_Timer_B2
//
// Description: Initializes/configures timer 2 and timer 2 interrupts
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Init_Timer_B2(void){
  TB2CTL = TBSSEL__SMCLK;               // SMCLK source
  TB2CTL |= TBCLR;                      // resets TB0R, clock divider, count direction
  TB2CTL |= MC__CONTINOUS;              // continuous up
  TB2CTL |= ID__8;                      // divide clock by 8
  
  TB2EX0 = TBIDEX__8;                   // divide clock by an additional 8
  
  TB2CCR0 = TB2CCR0_INTERVAL;           // CCR0
  //TB2CCTL0 |= CCIE;                     // CCR0 enable interrupt
  
  TB2CCR1 = TB2CCR1_INTERVAL;           // CCR1
  //TB2CCTL1 |= CCIE;                     // CCR1 enable interrupt
  
  TB2CCR2 = TB2CCR2_INTERVAL;           // CCR2
  //TB2CCTL2 |= CCIE;                   // CCR2 enable interrupt
  
  TB2CTL &= ~TBIE;                      // disable overflow interrupt
  TB2CTL &= ~TBIFG;                     // clear overflow interrupt flag 
}

//==============================================================================
// Function Name: Init_Timer_B3
//
// Description: Initializes/configures timer 3 and timer 3 PWM interrupt/duty
//              cycles
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Init_Timer_B3(void){
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 R_FORWARD
// TB3.2 P6.1 R_REVERSE
// TB3.3 P6.2 L_FORWARD
// TB3.4 P6.3 L_REVERSE
//------------------------------------------------------------------------------
  TB3CTL = TBSSEL__SMCLK;               // SMCLK
  TB3CTL |= MC__UP;                     // Up Mode
  TB3CTL |= TBCLR;                      // Clear TAR

  TB3CCR0 = WHEEL_PERIOD;               // PWM Period

  
  TB3CCTL1 = OUTMOD_7;                  // CCR1 reset/set
  RIGHT_FORWARD_SPEED = WHEEL_OFF;      // P6.0 Right Forward PWM duty cycle

  TB3CCTL2 = OUTMOD_7;                  // CCR1 reset/set
  RIGHT_REVERSE_SPEED = WHEEL_OFF;      // P6.0 Right Forward PWM duty cycle
  
  TB3CCTL3 = OUTMOD_7;                  // CCR3 reset/set
  LEFT_FORWARD_SPEED  = WHEEL_OFF;       // P6.2 Left Forward PWM duty cycle
  
  TB3CCTL4 = OUTMOD_7;                  // CCR1 reset/set
  LEFT_REVERSE_SPEED  = WHEEL_OFF;      // P6.0 Right Forward PWM duty cycle
}

