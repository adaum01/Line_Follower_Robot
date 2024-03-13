//------------------------------------------------------------------------------
//
//  Description: This is the header file macros.h, contains  definitions
//  Needs to be included in main.c, clocks.c, init.c, ports.c led.c (add as needed)
//
//  Alex Daum
//  September 14 2022
//  Built with IAR Embedded Workbench Version: (7.21.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Taken from main.c
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define BEGINNING               (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define TEST_PROBE           (0x01) // 3.0 TEST PROBE
#define RED_LED              (0x01) // 1.0 RED LED 0
#define GRN_LED              (0x40) // 6.6 GREEN LED

#define SCREEN_LIMIT           (10)
#define SCREEN_LIMIT_END       (11)
#define DISPLAY_LINE_1          (0)
#define DISPLAY_LINE_2          (1)
#define DISPLAY_LINE_3          (2)
#define DISPLAY_LINE_4          (3)
#define DISPLAY_LINE_TOTAL      (4)

#define ASCII_newline        ('\n')
#define ASCII_null           ('\0')
#define ASCII_0               ('0')
#define ASCII_p               ('p')
#define ASCII_o               ('o')
#define ASCII_q               ('q')
#define ASCII_w               ('w')
#define ASCII_s               ('s')
#define ASCII_a               ('a')
#define ASCII_d               ('d')
#define ASCII_f               ('f')
#define ASCII_I               ('I')
#define ASCII_P               ('P')
#define ASCII_plus            ('+')
#define ASCII_quote           ('"')
#define ASCII_period          ('.')

#define TENS_CONV              (10)

#define IOT                   ('I')
#define AUTO                  ('A')

// CLOCKS
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

// TIMER DEFINITIONS ===========================================================
#define TB0CCR0_INTERVAL   (50000)   // 8,000,000 / 4 / 8 / (1 / 200msec)
#define TB0CCR1_INTERVAL   (25000)   // 8,000,000 / 4 / 8 / (1 / 100msec)
#define TB0CCR2_INTERVAL   (50000)   // 8,000,000 / 4 / 8 / (1 / 200msec)

#define TB1CCR0_INTERVAL   (9000)   // 8,000,000 / 4 / 8 / (1 / 50msec)        //was 12500
#define TB1CCR1_INTERVAL   (50000)   // 8,000,000 / 4 / 8 / (1 / 200msec)
#define TB1CCR2_INTERVAL   (8000)   // 8,000,000 / 4 / 8 / (1 / 50msec)

#define TB2CCR0_INTERVAL   (50000)   // 8,000,000 / 8 / 8 / (1 / 200msec)
#define TB2CCR1_INTERVAL   (62500)   // 8,000,000 / 8 / 8 / (1 / 500msec)
#define TB2CCR2_INTERVAL   (62500)   // 8,000,000 / 8 / 8 / (1 / 500msec)

#define TB3CCR0_INTERVAL   (50000)   // 8,000,000 / 4 / 8 / (1 / 200msec)
#define TB3CCR1_INTERVAL   (12500)   // 8,000,000 / 4 / 8 / (1 / 50msec)
#define TB3CCR2_INTERVAL   (50000)   // 8,000,000 / 4 / 8 / (1 / 200msec)

#define NO_INTERRUPT            (0)
#define CCR1                    (2)
#define CCR2                    (4)
#define OVERFLOW               (14)

#define SW_TIMER_INTERRUPT      (5)
#define SEC_TIMER_INTERRUPT    (20)
#define SEC_5_TIMER_INTERRUPT  (10)
#define SCREEN_TIMER_INTERVAL   (2)
#define SCREEN_TIMER_INDEX_1    (6)
#define SCREEN_TIMER_INDEX_2    (7)
#define SCREEN_TIMER_INDEX_3    (8)
#define SCREEN_TIMER_INDEX_4    (9)
#define ON                    ('1')
#define OFF                   ('0')

#define NEXT_CMD_COUNT_LIMIT            (2)
#define NEXT_CMD_COUNT_LONG_LIMIT      (12)
#define NEXT_CMD_PING                  (14)

#define CIRCLE_STOP_TIME               (47)

#define DURATION_ADJUSTMENT             (1)
#define DURATION_WAIT_ADJUSTMENT        (3)

// PWM DEFINITIONS =============================================================
#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define RIGHT_REVERSE_SPEED (TB3CCR2)
#define LEFT_FORWARD_SPEED  (TB3CCR3)
#define LEFT_REVERSE_SPEED  (TB3CCR4)

#define WHEEL_OFF              (0)
#define WHEEL_PERIOD       (20000)     
#define RIGHT_FORWARD       (3000)    
#define LEFT_FORWARD        (3000)     

#define RIGHT_SLOW          (2800)
#define LEFT_SLOW           (2800)

#define RIGHT_ALIGN         (2500)
#define LEFT_ALIGN          (2500)

#define RIGHT_ARC           (3000)
#define LEFT_ARC            (2850)

#define RIGHT_FOLLOW_LIMIT  (3000)
#define LEFT_FOLLOW_LIMIT   (3000)

//IOT WHEEL SPEEDS =============================================================
#define RIGHT_FORWARD_FAST  (15000)
#define LEFT_FORWARD_FAST   (15000)

#define RIGHT_TURN           (5000)
#define LEFT_TURN            (5000)

#define RIGHT_REVERSE       (15000)
#define LEFT_REVERSE        (15000)

// PID =========================================================================
#define RIGHT_TARGET           (55)
#define RIGHT_P_CONST         (400)
//#define RIGHT_I_CONST         (1)
#define RIGHT_D_CONST         (400)

#define LEFT_TARGET            (55)
#define LEFT_P_CONST          (400)
//#define LEFT_I_CONST          (1)
#define LEFT_D_CONST          (400)

#define LEFT                    ('L')
#define RIGHT                   ('R')

// MOVEMENT STATES =============================================================
#define IDLE                    ('I')

#define CONFIG_HIGH             ('1')
#define CONFIG_LOW              ('2')
#define CONFIG_DONE             ('3')

#define IOT_ACTIVE              ('4')

#define BLACK_LINE_START        ('S')
#define FIND_BOARD              ('F')
#define RUN                     ('R')
#define DETECTED                ('D')
#define DETECTED_DELAY          ('W')
#define ALIGN                   ('A')
#define ALIGN_DELAY             ('X')
#define LINE_FOLLOW             ('L')
#define CIRCLE_WAIT             ('C')

#define END_PROGRAM_START       ('!')
#define END_PROGRAM_TURN        ('@')
#define END_PROGRAM_TURN_WAIT   ('#')
#define END_PROGRAM_EXIT        ('$')
#define END_PROGRAM_FINISH      ('%')

// ADC/DETECTOR VALUES =============================================================
#define DETECT_THRESHOLD         (40)
#define RIGHT_DETECT_ADJUSTMENT   (7)
#define LEFT_DETECT_ADJUSTMENT    (7)

#define DETECT_DISPLAY_LINE       (1)
#define DETECT_DISPLAY_LEFT       (6)
#define DETECT_DISPLAY_RIGHT      (0)
#define LEFT_TARGET_INDEX_1       (4)
#define LEFT_TARGET_INDEX_2       (5)
#define RIGHT_TARGET_INDEX_1      (8)
#define RIGHT_TARGET_INDEX_2      (9)

#define LEFT_DETECTOR_CHANNEL  (0x00)
#define RIGHT_DETECTOR_CHANNEL (0x01)
#define THUMB_WHEEL_CHANNEL    (0x02)
#define UCA0_RANGE             (0x08)

#define DIVIDE_BY_4               (2)
#define DIVIDE_BY_16              (4)

#define ADC_CHAR_LIMIT          (4)
#define ADC_CHAR_THOUSANDS      (0)
#define ADC_CHAR_HUNDREDS       (1)
#define ADC_CHAR_TENS           (2)
#define ADC_CHAR_ONES           (3)
#define ADC_VAL_INCREMENT       (1)
#define HEX_0                (0x30)

#define THOUSANDS_LIMIT       (999)
#define THOUSANDS_DECREMENT  (1000)
#define HUNDREDS_LIMIT         (99)
#define HUNDREDS_DECREMENT    (100)
#define TENS_LIMIT              (9)
#define TENS_DECREMENT         (10)

#define LINE_DECREMENT          (1)

// SERIAL DEFINITIONS ==========================================================
#define SMALL_RING_SIZE          (32)
#define DBL_RING_SIZE            (64)

#define BAUD_115200             ('A')
#define BAUD_460800             ('B')

#define RECEIVE_INTERRUPT         (2)
#define TRANSMIT_INTERRUPT        (4)

#define HEX_ZERO               (0x00)
#define BRW_BAUD_115200           (4)
#define MCTLW_BAUD_115200    (0x5551)

//IOT ===================================================================
#define IOT_STARTUP             ('1')
#define SAVE_OFF                ('2')
#define HARD_RESET              ('3')
#define SOFT_RESET              ('4')
#define SSID_REQ                ('S')
#define IP_REQ                  ('I')
#define PING                    ('P')
#define DONE                    ('D')
#define STATE_7                 ('7')

#define CMD_CHAR                ('^')
#define PASS_1_NUM              ('2')
#define PASS_2_NUM              ('4')
#define PASS_3_NUM              ('0')
#define PASS_4_NUM              ('1')
#define PASS_1_OFFSET             (1)
#define PASS_2_OFFSET             (2)
#define PASS_3_OFFSET             (3)
#define PASS_4_OFFSET             (4)
#define IP_CHECK_INDEX_1         (10)
#define IP_CHECK_INDEX_2         (11)

#define CMD_CHAR_OFFSET           (5)
#define CMD_CHAR_DISPLAY_INDEX    (0)
#define CMD_PAD_NUM_OFFSET        (6)
#define PAD_NUM_DISPLAY_INDEX     (9)
#define DURATION_10_OFFSET        (6)
#define DURATION_1_OFFSET         (7)
#define DURATION_10_DISPLAY_INDEX (1)
#define DURATION_1_DISPLAY_INDEX  (2)

#define IP_GROUP_1                (1)
#define IP_GROUP_2                (2)

#define IOT_MSG_LENGTH           (30)