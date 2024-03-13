//------------------------------------------------------------------------------
//
//  File Name: serial.c
//
//  Description: This file contains the initialization of serial port UCA0
//               initializes to a baudrate of 115,200
//
//  Alex Daum
//  Nov 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include "ports.h"
#include <string.h>

//UCA0 side
volatile unsigned char PC_2_IOT[SMALL_RING_SIZE];
volatile unsigned int usb_rx_wr;
unsigned int usb_rx_rd;
unsigned int direct_usb;

//UCA1 side
volatile unsigned char IOT_2_PC[SMALL_RING_SIZE];
volatile unsigned int iot_rx_wr;  
unsigned int iot_rx_rd;
unsigned int direct_iot;


//==============================================================================
// Function Name: Init_Serial_UCA0
//
// Description: Initializes Serial Port UCA0
//
// Author: Alex Daum
// Date: Nov 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void Init_Serial_UCA0(void){
  int i;
  usb_rx_wr = BEGINNING;
  usb_rx_rd = BEGINNING;
                   
  for(i=BEGINNING; i<SMALL_RING_SIZE; i++){    
    PC_2_IOT[i] = HEX_ZERO;     //clear TX ring buffer
  }
  iot_rx_wr = BEGINNING;
  iot_rx_rd = BEGINNING;
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK   Baudrate UCOS16 UCBRx UCFx UCSx  neg  pos   neg   pos
// 8000000    4800      1   104    2  0xD6 -0.08 0.04 -0.10 0.14
// 8000000    9600      1    52    1  0x49 -0.08 0.04 -0.10 0.14
// 8000000   19200      1    26    0  0xB6 -0.08 0.16 -0.28 0.20
// 8000000   57600      1     8   10  0xF7 -0.32 0.32 -1.00 0.36
// 8000000  115200      1     4    5  0x55 -0.80 0.64 -1.12 1.76
// 8000000  460800      0    17    0  0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
  UCA0CTLW0 =   RESET_STATE;
  UCA0CTLW0 |=  UCSWRST ;               // Put eUSCI in reset
  UCA0CTLW0 |=  UCSSEL__SMCLK;          // Set SMCLK as fBRCLK
  UCA0CTLW0 &= ~UCMSB;                  // MSB, LSB select
  UCA0CTLW0 &= ~UCSPB;                  // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
  UCA0CTLW0 &= ~UCPEN;                  // No Parity
  UCA0CTLW0 &= ~UCSYNC;
  UCA0CTLW0 &= ~UC7BIT;
  UCA0CTLW0 |=  UCMODE_0;
  
  
  // BRCLK   Baudrate UCOS16 UCBRx UCFx UCSx  neg  pos   neg  pos
  // 8000000  115200      1     4    5  0x55 -0.80 0.64 -1.12 1.76
  // UCA?MCTLW = UCSx + UCFx + UCOS16
  UCA0BRW = BRW_BAUD_115200;            // 115,200 baud configuration
  UCA0MCTLW = MCTLW_BAUD_115200;
  UCA0CTLW0 &= ~UCSWRST ;               // release from reset
  UCA0IE |= UCRXIE;                     // Enable RX interrupt
//------------------------------------------------------------------------------
}