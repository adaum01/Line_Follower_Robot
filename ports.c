//-------------------------------------------------------------------------------
//
//  File Name: ports.c
//
//  Description: This file contains the the Port Definintion, Configuration, 
//               and Initialization using functions Init_Ports() and
//               Init_Port_X() for ports 1-6
//
//  Alex Daum
//  Sept 2022
//  Built with IAR Embedded Workbench Version: V7.21.1
//-------------------------------------------------------------------------------
//  Globals - none
//-------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include <string.h>
#include "macros.h"
#include "ports.h"

void Init_Port_1(void){
//------------------------------------------------------------
//  Init_Port 1 Program
//  Configures Port 1 pins to preferred settings when called.
//
//  Globals used - None
//  Inputs - None, Outputs - None
//-------------------------------------------------------------
  P1OUT = LOW_VAL;              // P1 set Low
  P1DIR = LOW_VAL;              // Set P1 direction to output
  
  P1SEL0 &= ~RED_LED;           //GPIO operation
  P1SEL1 &= ~RED_LED;           //GPIO operation
  //P1OUT
  P1DIR  |=  RED_LED;           //Direction - Output
    
  P1SEL0 &= ~A1_SEEED;          //GPIO operation
  P1SEL1 &= ~A1_SEEED;          //GPIO operation
  //P1OUT
  P1DIR  &= ~A1_SEEED;          //Direction - Input
    
  P1SEL0 |=  V_DETECT_L;        //Configured for ADC
  P1SEL1 |=  V_DETECT_L;        //Configured for ADC
  //P1OUT
  P1DIR  |=  V_DETECT_L;        //Direction - Output
    
  P1SEL0 |=  V_DETECT_R;        //Configured for ADC
  P1SEL1 |=  V_DETECT_R;        //Configured for ADC
  //P1OUT
  P1DIR  |=  V_DETECT_R;        //Direction - Output
    
  P1SEL0 &= ~A4_SEEED;          //GPIO operation
  P1SEL1 &= ~A4_SEEED;          //GPIO operation
  //P1OUT
  P1DIR  &= ~A4_SEEED;          //Direction - Input
    
  P1SEL0 |=  V_THUMB;           //Configured for ADC
  P1SEL1 |=  V_THUMB;           //Configured for ADC
  //P1OUT
  P1DIR  |=  V_THUMB;           //Direction - Output
    
  P1SEL0 |=  UCA0RXD;        //Configured for Serial Communication
  P1SEL1 &= ~UCA0RXD;        //Configured for Serial Communication
  //P1OUT
  //P1DIR  &= ~UCA0RXD;
    
  P1SEL0 |=  UCA0TXD;        //Configured for Serial Communication
  P1SEL1 &= ~UCA0TXD;        //Configured for Serial Communication
  //P1OUT
  //P1DIR  &= ~UCA0TXD;
  //-------------------------------------------------------------
}


void Init_Port_2(void){
//------------------------------------------------------------
//  Init_Port 2 Program
//  Configures Port 2 pins to preferred settings when called.
//
//  Globals used - None
//  Inputs - None, Outputs - None
//-------------------------------------------------------------
  P2OUT = LOW_VAL;              // P2 set Low
  P2DIR = LOW_VAL;              // Set P2 direction to output
  
  P2SEL0 &= ~RESET_LCD;         // RESET_LCD GPIO operation
  P2SEL1 &= ~RESET_LCD;         // RESET_LCD GPIO operation
  P2OUT  &= ~RESET_LCD;         // Initial Value = Low / Off
  P2DIR  |=  RESET_LCD;         // Direction = output
  
  P2SEL0 &= ~Check_Bat;         // Check_Bat GPIO operation
  P2SEL1 &= ~Check_Bat;         // Check_Bat GPIO operation
  P2OUT  &= ~Check_Bat;         // Initial Value = Low / Off
  P2DIR  |=  Check_Bat;         // Direction = output
  
  P2SEL0 &= ~IR_LED;            // P2_2 GPIO operation
  P2SEL1 &= ~IR_LED;            // P2_2 GPIO operation
  P2OUT  &= ~IR_LED;            // Initial Value = Low / Off
  P2DIR  |=  IR_LED;            // Direction = input
  
  P2SEL0 &= ~SW2;               // SW2 Operation
  P2SEL1 &= ~SW2;               // SW2 Operation
  P2OUT  |=  SW2;               // Configure pullup resistor
  P2DIR  &= ~SW2;               // Direction = input
  P2REN  |=  SW2;               // Enable pullup resistor
  P2IES  |=  SW2;               // SW2 Hi/Lo edge interrupt
  P2IFG  &= ~SW2;               // IFG SW2 cleared
  P2IE   |=  SW2;               // SW2 interrupt Enabled
  
  P2SEL0 &= ~IOT_RUN_CPU;       // IOT_RUN_CPU GPIO operation
  P2SEL1 &= ~IOT_RUN_CPU;       // IOT_RUN_CPI GPIO operation
  //P2OUT  &= ~IOT_RUN_CPU;       // Initial Value = Low / Off
  P2DIR  |=  IOT_RUN_CPU;       // Direction = input
  
  P2SEL0 &= ~DAC_ENB;           // DAC_ENB GPIO operation
  P2SEL1 &= ~DAC_ENB;           // DAC_ENB GPIO operation
  P2OUT  |=  DAC_ENB;           // Initial Value = High
  P2DIR  |=  DAC_ENB;           // Direction = output
  
  P2SEL0 &= ~LFXOUT;            // LFXOUT Clock operation
  P2SEL1 |=  LFXOUT;            // LFXOUT Clock operation
  
  P2SEL0 &=  ~LFXIN;            // LFXIN Clock operation
  P2SEL1 |=   LFXIN;            // LFXIN Clock operation
  //-----------------------------------------------------------
}

void Init_Port_3(void){
      P3OUT = LOW_VAL;              // P3 set Low
      P3DIR = LOW_VAL;              // Set P3 direction to output
  
      P3SEL0 &= ~TEST_PROBE;        //GPIO operation
      P3SEL1 &= ~TEST_PROBE;        //GPIO operation
      //P3OUT
      P3DIR  &= ~TEST_PROBE;        //Direction - Input
    
      P3SEL0 &= ~OA2O;        //GPIO operation
      P3SEL1 &= ~OA2O;        //GPIO operation
      //P3OUT
      P3DIR  &= ~OA2O;        //Direction - Input
    
      P3SEL0 &= ~OA2N;        //GPIO operation
      P3SEL1 &= ~OA2N;        //GPIO operation
      //P3OUT
      P3DIR  &= ~OA2N;        //Direction - Input
    
      P3SEL0 &= ~OA2P;        //GPIO operation
      P3SEL1 &= ~OA2P;        //GPIO operation
      //P3OUT
      P3DIR  &= ~OA2P;        //Direction - Input
    
      P3SEL0 |=  SMCLK;
      P3SEL1 &= ~SMCLK;
      //P3OUT  |=  SMCLK;
      P3DIR  |=  SMCLK;
    
      P3SEL0 &= ~DAC_CNTL;        //GPIO operation
      P3SEL1 &= ~DAC_CNTL;        //GPIO operation
      //P3OUT
      P3DIR  &= ~DAC_CNTL;        //Direction - Input
    
      P3SEL0 &= ~IOT_LINK_CPU;        //GPIO operation
      P3SEL1 &= ~IOT_LINK_CPU;        //GPIO operation
      //P3OUT
      P3DIR  |=  IOT_LINK_CPU;        //SWITCHED TO OUTPUT
    
      P3SEL0 &= ~IOT_EN_CPU;        //GPIO operation
      P3SEL1 &= ~IOT_EN_CPU;        //GPIO operation
      P3OUT  &= ~IOT_EN_CPU;        //SET INIT COND TO 0
      P3DIR  |=  IOT_EN_CPU;        //SWITCHED TO OUTPUT
}


void Init_Port_4(void){
//------------------------------------------------------------
//  Init_Port 4 Program
//  Configures Port 4 pins to preferred settings when called.
//
//  Globals used - None
//  Inputs - None, Outputs - None
//-------------------------------------------------------------
  P4OUT = LOW_VAL;              // P4 set Low
  P4DIR = LOW_VAL;              // Set P4 direction to output
  
  P4SEL0 &= ~DAC_CNTL1;         // DAC_CNTL1 GPIO operation
  P4SEL1 &= ~DAC_CNTL1;         // DAC_CNTL1 GPIO operation
  P4OUT  |=  DAC_CNTL1;         // Set DAC_CNTL1 Off [High]
  P4DIR  |=  DAC_CNTL1;         // Set DAC_CNTL1 direction to output
  
  P4SEL0 &= ~SW1;               // SW1 GPIO operation
  P4SEL1 &= ~SW1;               // SW1 GPIO operation
  P4OUT  |=  SW1;               // Configure pullup resistor
  P4DIR  &= ~SW1;               // Direction = input
  P4REN  |=  SW1;               // Enable pullup resistor
  P4IES  |=  SW1;               // P4.2 Hi/Lo edge interrupt
  P4IFG  &= ~SW1;               // Clear all P2.6 interrupt flags
  P4IE   |=  SW1;               // P4.2 interrupt enabled
  
  P4SEL0 |=  UCA1TXD;           // USCI_A1 UART operation
  P4SEL1 &= ~UCA1TXD;           // USCI_A1 UART operation
  
  P4SEL0 |=  UCA1RXD;           // USCI_A1 UART operation
  P4SEL1 &= ~UCA1RXD;           // USCI_A1 UART operation
  
  P4SEL0 &= ~UCB1_CS_LCD;       // UCB1_CS_LCD GPIO operation
  P4SEL1 &= ~UCB1_CS_LCD;       // UCB1_CS_LCD GPIO operation
  P4OUT  |=  UCB1_CS_LCD;       // Set SPI_CS_LCD Off [High]
  P4DIR  |=  UCB1_CS_LCD;       // Set SPI_CS_LCD direction to output
  
  P4SEL0 |=  UCB1CLK;           // UCB1CLK SPI BUS operation
  P4SEL1 &= ~UCB1CLK;           // UCB1CLK SPI BUS operation
  
  P4SEL0 |=  UCB1SIMO;          // UCB1SIMO SPI BUS operation
  P4SEL1 &= ~UCB1SIMO;          // UCB1SIMO SPI BUS operation
  
  P4SEL0 |=  UCB1SOMI;          // UCB1SOMI SPI BUS operation
  P4SEL1 &= ~UCB1SOMI;          // UCB1SOMI SPI BUS operation 
  //-------------------------------------------------------------
}

void Init_Port_5(void){
//------------------------------------------------------------
//  Init_Port 5 Program
//  Configures Port 5 pins to preferred settings when called.
//
//  Globals used - None
//  Inputs - None, Outputs - None
//-------------------------------------------------------------
  P5OUT = LOW_VAL;              // P5 set Low
  P5DIR = LOW_VAL;              // Set P5 direction to output
  
  P5SEL0 &= ~V_BAT;             //GPIO operation
  P5SEL1 &= ~V_BAT;             //GPIO operation
  //P5OUT
  P5DIR  &= ~V_BAT;             //Direction - Input
    
  P5SEL0 &= ~V_5;               //GPIO operation
  P5SEL1 &= ~V_5;               //GPIO operation
  //P5OUT
  P5DIR  &= ~V_5;               //Direction - Input
    
  P5SEL0 &= ~V_DAC;             //GPIO operation
  P5SEL1 &= ~V_DAC;             //GPIO operation
  //P5OUT
  P5DIR  &= ~V_DAC;             //Direction - Input
    
  P5SEL0 &= ~V_3_3;             //GPIO operation
  P5SEL1 &= ~V_3_3;             //GPIO operation
  //P5OUT
  P5DIR  &= ~V_3_3;             //Direction - Input
    
  P5SEL0 &= ~IOT_BOOT_CPU;      //GPIO operation
  P5SEL1 &= ~IOT_BOOT_CPU;      //GPIO operation
  P5OUT  |=  IOT_BOOT_CPU;      //SET INIT COND TO 1
  P5DIR  |=  IOT_BOOT_CPU;      //SET TO OUTPUT
  //-------------------------------------------------------------
}

void Init_Port_6(void){
//------------------------------------------------------------
//  Init_Port 2 Program
//  Configures Port 2 pins to preferred settings when called.
//
//  Globals used - None
//  Inputs - None, Outputs - None
//-------------------------------------------------------------
  P6OUT = LOW_VAL;              // P6 set Low
  P6DIR = LOW_VAL;              // Set P6 direction to output
  
  P6SEL0 |=  R_FORWARD;         // CONFIGURED FOR PWM
  P6SEL1 &= ~R_FORWARD;         //
  //P6OUT  &= ~R_FORWARD;       //
  P6DIR  |=  R_FORWARD;         //
    
  P6SEL0 |=  R_REVERSE;         // CONFIGURED FOR PWM
  P6SEL1 &= ~R_REVERSE;         //
  //P6OUT  &= ~R_REVERSE;       //
  P6DIR  |=  R_REVERSE;         //
    
  P6SEL0 |=  L_FORWARD;         // CONFIGURED FOR PWM
  P6SEL1 &= ~L_FORWARD;         //
  //P6OUT  &= ~L_FORWARD;       //
  P6DIR  |=  L_FORWARD;         //
    
  P6SEL0 |=  L_REVERSE;         // CONFIGURED FOR PWM
  P6SEL1 &= ~L_REVERSE;         //
  //P6OUT  &= ~L_REVERSE;       //
  P6DIR  |=  L_REVERSE;         //
    
  P6SEL0 &= ~LCD_BACKLITE;      //GPIO operation
  P6SEL1 &= ~LCD_BACKLITE;      //GPIO operation
  P6OUT  &= ~LCD_BACKLITE;      //Output low
  P6DIR  |= LCD_BACKLITE;       //Direction - Output
    
  P6SEL0 &= ~P6_5;              //GPIO operation
  P6SEL1 &= ~P6_5;              //GPIO operation
  //P6OUT
  P6DIR  &= ~P6_5;              //Direction - Input
    
  P6SEL0 &= ~GRN_LED;           //GPIO operation
  P6SEL1 &= ~GRN_LED;           //GPIO operation
  //P6OUT
  P6DIR  |=  GRN_LED;           //Direction - Output
  //-------------------------------------------------------------
}


void Init_Ports(void){
//------------------------------------------------------------
//  Init_Ports Program
//  Calls Init_Port_X() for ports 1-6
//
//  Globals used - None
//  Inputs - None, Outputs - None
//------------------------------------------------------------ 
  Init_Port_1();
  Init_Port_2();
  Init_Port_3();
  Init_Port_4();
  Init_Port_5();
  Init_Port_6();
}
