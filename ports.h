//------------------------------------------------------------------------------
//
//  Description: This is the header file for port definitions
//
//  Alex Daum
//  September 15\ 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#define LOW_VAL                 (0x00)
//Definitions for Port 1 Pins
#define RED_LED                 (0x01) // 0 RED LED 0
#define A1_SEEED                (0x02) // 1 A1_SEEED
#define V_DETECT_L              (0x04) // 2 V_DETECT_L
#define V_DETECT_R              (0x08) // 3 V_DETECT_R
#define A4_SEEED                (0x10) // 4 A4_SEEED
#define V_THUMB                 (0x20) // 5 V_THUMB
#define UCA0RXD                 (0x40) // 6 Back Channel UCA0RXD
#define UCA0TXD                 (0x80) // 7 Back Channel UCA0TXD
//Definitions for Port 2 Pins
#define RESET_LCD               (0x01) // 0 RESET LCD
#define Check_Bat               (0x02) // 1 Check ADC Voltages
#define IR_LED                  (0x04) // 2 IR_LED
#define SW2                     (0x08) // 3 SW2
#define IOT_RUN_CPU             (0x10) // 4 IOT_RUN_CPU
#define DAC_ENB                 (0x20) // 5 DAC_ENB
#define LFXOUT                  (0x40) // 6 XOUTR
#define LFXIN                   (0x80) // 7 XINR
//Definitions for Port 3 Pins
#define TEST_PROBE              (0x01) // 0 TEST PROBE
#define OA2O                    (0x02) // 1 OA20
#define OA2N                    (0x04) // 2 OA2N
#define OA2P                    (0x08) // 3 OA2P
#define SMCLK                   (0x10) // 4 SMCLK
#define DAC_CNTL                (0x20) // 5 DAC signal from Processor
#define IOT_LINK_CPU            (0x40) // 6 IOT_LINK_CPU
#define IOT_EN_CPU              (0x80) // 7 IOT_EN_CPU
//Definitions for Port 4 Pins
#define DAC_CNTL1               (0x01) // 0 DAC_CNTL
#define SW1                     (0x02) // 1 SW1
#define UCA1RXD                 (0x04) // 2 Back Channel UCA1RXD
#define UCA1TXD                 (0x08) // 3 Back Channel UCA1TXD
#define UCB1_CS_LCD             (0x10) // 4 Chip Select
#define UCB1CLK                 (0x20) // 5 SPI mode - clock output-UCB1CLK
#define UCB1SIMO                (0x40) // 6 UCB1SIMO
#define UCB1SOMI                (0x80) // 7 UCB1SOMI
//Definitions for Port 5 Pins
#define V_BAT                   (0x01) // 0 V_BAT
#define V_5                     (0x02) // 1 V_5
#define V_DAC                   (0x04) // 2 V_DAC
#define V_3_3                   (0x08) // 3 V_3_3
#define IOT_BOOT_CPU            (0x10) // 4 IOT_BOOT_CPU
//Definitions for Port 6 Pins
#define R_FORWARD               (0x01) // 0 R_FORWARD
#define R_REVERSE               (0x02) // 1 R_REVERSE
#define L_FORWARD               (0x04) // 2 L_FORWARD
#define L_REVERSE               (0x08) // 3 L_REVERSE
#define LCD_BACKLITE            (0x10) // 4 LCD_BACKLITE
#define P6_5                    (0x20) // 5 P6_5
#define GRN_LED                 (0x40) // 6 GRN_LED
