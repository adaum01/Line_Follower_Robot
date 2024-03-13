//-------------------------------------------------------------------------------
//
//  File Name: wheels.c
//
//  Description: This file contains the Wheel Motor Functions, handles disabling
//               of motors, driving car in particular directions, safety checks
//               to prevent FET/motor damage, and the state machine for the 
//               black line following function (which contains PID calculations)
//
//
//  Alex Daum
//  Sept 2022
//  Built with IAR Embedded Workbench Version: V7.21.1
//-------------------------------------------------------------------------------
//  Globals - none
//-------------------------------------------------------------------------------
//  motors_off()
//  Forward_Drive()
//  Reverse_Drive()
//-------------------------------------------------------------------------------

#include  "functions.h"
#include  "msp430.h"
#include "macros.h"
#include "ports.h"
#include <string.h>

extern char display_line[4][11];
extern volatile unsigned char display_changed;

volatile char state = IDLE;
extern volatile unsigned int Left_Detect_Hold;
extern volatile unsigned int Right_Detect_Hold;

extern volatile char sec_flag;
extern volatile char sec_5_flag;

unsigned int Right_Detect_Hold_Prev;
int right_speed_val;
int right_error;
int right_p_term;
int right_i_term;
int right_d_term;
int Right_PID;

unsigned int Left_Detect_Hold_Prev;
int left_speed_val;
int left_error;
int left_p_term;
int left_i_term;
int left_d_term;
int Left_PID;

unsigned char last_direction;
extern volatile unsigned char timer_flag;

extern volatile unsigned int right_BL_target;
extern volatile unsigned int left_BL_target;
extern volatile unsigned int right_white_target;
extern volatile unsigned int left_white_target;

//extern unsigned char auto_active_flag;
extern unsigned char IOT_AUTO_active_flag;


//==============================================================================
// Function Name: motors_off
//
// Description: Disables left and right motors
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void motors_off(void){
  RIGHT_FORWARD_SPEED = WHEEL_OFF;      //
  LEFT_FORWARD_SPEED  = WHEEL_OFF;      //set all wheel speeds to 0
  RIGHT_REVERSE_SPEED = WHEEL_OFF;      //
  LEFT_REVERSE_SPEED  = WHEEL_OFF;      //
}

//==============================================================================
// Function Name: check_right_oopsie
//
// Description: Safety check to determine if right motor is set to a forward
//              and reverse speed simultaneously, disable all motors if true
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void check_right_oopsie(void){
  if((RIGHT_FORWARD_SPEED != WHEEL_OFF) && (RIGHT_REVERSE_SPEED != WHEEL_OFF)){ //if right forward and reverse active at same time
    motors_off();                                                               //disable all motors
  }
}

//==============================================================================
// Function Name: check_left_oopsie
//
// Description: Safety check to determine if left motor is set to a forward
//              and reverse speed simultaneously, disable all motors if true
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void check_left_oopsie(void){
  if((LEFT_FORWARD_SPEED != WHEEL_OFF) && (LEFT_REVERSE_SPEED != WHEEL_OFF)){   //if left forward and reverse active at same time
    motors_off();                                                               //disable all motors
  }
}


//==============================================================================
// MOVEMENT FUNCTIONS ==========================================================
//==============================================================================

//==============================================================================
// Function Name: motor_forward
//
// Description: Set both motors to forward speed, perform safety check
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void motor_forward(void){
  RIGHT_FORWARD_SPEED = RIGHT_FORWARD_FAST;             //set right motor to right pwm speed
  LEFT_FORWARD_SPEED  = LEFT_FORWARD_FAST;              //set left motor to left pwm speed
  check_right_oopsie();         //safety checks
  check_left_oopsie();          //
}

//==============================================================================
// Function Name: motor_reverse
//
// Description: Set both motors to reverse speed, perform safety check
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void motor_reverse(void){
  RIGHT_REVERSE_SPEED = RIGHT_REVERSE;          //set right motor to right pwm speed
  LEFT_REVERSE_SPEED  = LEFT_REVERSE;           //set left motor to left pwm speed
  check_right_oopsie();         //safety checks
  check_left_oopsie();          //safety checks
}

//==============================================================================
// Function Name: motor_forward
//
// Description: Set left motor to forward speed, right motor to reverse speed,  
//              perform safety check
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void motor_turn_left(void){
  RIGHT_REVERSE_SPEED = RIGHT_TURN;             //set right motor to right pwm speed
  LEFT_FORWARD_SPEED  = LEFT_TURN;              //set left motor to left pwm speed
  check_right_oopsie();         //safety checks
  check_left_oopsie();          //safety checks
}

//==============================================================================
// Function Name: motor_forward
//
// Description: Set right motor to forward speed, left motor to reverse speed,  
//              perform safety check
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void motor_turn_right(void){
  RIGHT_FORWARD_SPEED = RIGHT_TURN;             //set right motor to right pwm speed
  LEFT_REVERSE_SPEED  = LEFT_TURN;              //set left motor to left pwm speed
  check_right_oopsie();         //safety checks
  check_left_oopsie();          //safety checks
}


//==============================================================================
// BLACK LINE MOVEMENT STATE MACHINE ===========================================
//==============================================================================
//==============================================================================
// Function Name: movement
//
// Description: State machine for black line program, configures line detection,
//              performs line searching, intercepting, alignment, and following
//              Also performs exit sequence
//
// Author: Alex Daum
// Date: Sept 2022
// Built with IAR Embedded Workbench Version: V7.21.1
//==============================================================================
void movement(void){
  switch(state){
    case IDLE:
      TB1CCTL0 |= CCIE;                       //enable ADC sampling timer interrupt
      motors_off();                           //disable motors as safety precaution
      break;
      
    case CONFIG_HIGH:
      P2OUT |= IR_LED;                                          //enable IR LEDs
      strcpy(display_line[DISPLAY_LINE_4], "ConfigHigh");       //display state to screen
      display_changed = TRUE;
      break;
      
    case CONFIG_LOW:
      strcpy(display_line[DISPLAY_LINE_4], "ConfigLow ");       //update state to screen
      display_changed = TRUE;
      break;
      
    case CONFIG_DONE:
      P2OUT &= ~IR_LED;         //disable IR LEDs
      TB1CCTL0 &= ~CCIE;        //stop ADC sampling interrupt
      
      strcpy(display_line[DISPLAY_LINE_2], "BL: LT  RT");       //display ADC readings format
      strcpy(display_line[DISPLAY_LINE_3], "W:  LT  RT");       //
      strcpy(display_line[DISPLAY_LINE_4], "CONFIGDONE");       //update state to screen
      
      display_line[DISPLAY_LINE_2][LEFT_TARGET_INDEX_1] = left_BL_target/TENS_CONV + ASCII_0;   //display tens position of left black reading
      display_line[DISPLAY_LINE_2][LEFT_TARGET_INDEX_2] = left_BL_target%TENS_CONV + ASCII_0;   //display ones position of left black reading
      display_line[DISPLAY_LINE_2][RIGHT_TARGET_INDEX_1] = right_BL_target/TENS_CONV + ASCII_0; //display tens position of right black reading
      display_line[DISPLAY_LINE_2][RIGHT_TARGET_INDEX_2] = right_BL_target%TENS_CONV + ASCII_0; //display ones position of right black reading
      
      display_line[DISPLAY_LINE_3][LEFT_TARGET_INDEX_1] = left_white_target/TENS_CONV + ASCII_0;        //display tens position of left white reading
      display_line[DISPLAY_LINE_3][LEFT_TARGET_INDEX_2] = left_white_target%TENS_CONV + ASCII_0;        //display ones position of left white reading
      display_line[DISPLAY_LINE_3][RIGHT_TARGET_INDEX_1] = right_white_target/TENS_CONV + ASCII_0;      //display tens position of right white reading
      display_line[DISPLAY_LINE_3][RIGHT_TARGET_INDEX_2] = right_white_target%TENS_CONV + ASCII_0;      //display ones position of right white reading
      
      display_changed = TRUE;
      break;
      
    case IOT_ACTIVE:
      clear_screen();                                           //clear LCD screen
      strcpy(display_line[DISPLAY_LINE_2], "Loading...");       //display waiting text
      display_changed = TRUE;
      TB1CCTL0 &= ~CCIE;                       //disable sampling timer
      IOT_AUTO_active_flag = IOT;              //start IOT automatic messages
      break;
      
    case BLACK_LINE_START:
      motors_off();                     //disable motors
      
      P6OUT |= LCD_BACKLITE;                                    //enable backlite
      strcpy(display_line[DISPLAY_LINE_1], "BL START  ");       //
      strcpy(display_line[DISPLAY_LINE_2], "   ALEX   ");       //display information to screen
      strcpy(display_line[DISPLAY_LINE_3], "   DAUM   ");       //
      display_changed = TRUE;
      
      sec_5_flag = ON;          //set flag high, will be set low once done
      TB2CCTL1 |=  CCIE;        //enables 5 second interrupt timer
      state = FIND_BOARD;       //set to next state
      break;
      
    case FIND_BOARD:
      if(sec_5_flag == OFF){                    //once 5 seconds has elapsed
        P6OUT &= ~LCD_BACKLITE;                 //disable backlite
        P2OUT |= IR_LED;                        //enable IR leds
        TB1CCTL0 |= CCIE;                       //start taking ADC measurements
        RIGHT_FORWARD_SPEED = RIGHT_ARC; //set right motor to PWM arc value
        LEFT_FORWARD_SPEED = LEFT_ARC;   //Set left motor to PWM arc value
        if((Left_Detect_Hold < left_white_target) || (Right_Detect_Hold < right_white_target)){ //if either detector reads below the white board threshold
          state = RUN;                                                                          //on board, begin searching for black line
        }
      }
      break;
      
    case RUN:
      RIGHT_FORWARD_SPEED = RIGHT_FORWARD;      //set right motor to straight PWM value
      LEFT_FORWARD_SPEED  = LEFT_FORWARD;       //set left motor to straight PWM value
      check_right_oopsie();     //safety check
      check_left_oopsie();      //safety check
      
      if((Left_Detect_Hold > DETECT_THRESHOLD) || (Right_Detect_Hold > DETECT_THRESHOLD)){ //check sampled values, if black line value read by detectors
        motors_off();                                                                      //stop motors
        state = DETECTED;                                                                  //prepare for alignment
      }
      break;
      
    case DETECTED:
      P6OUT |= LCD_BACKLITE;                                    //enable backlite
      strcpy(display_line[DISPLAY_LINE_1], "INTERCEPT ");       //display information to screen
      display_changed = TRUE;
      
      motors_off();             //disable motors as a safety precaution
      
      sec_5_flag = ON;          //set 5 second wait flag high
      TB2CCTL1 |=  CCIE;        //enables 5 second interrupt timer
      state = DETECTED_DELAY;   //set to next state
      break;
      
    case DETECTED_DELAY:
      if(sec_5_flag == OFF){    //once 5 seconds has elapsed
        state = ALIGN;          //set to next state
      }
      break;
      
    case ALIGN:
      strcpy(display_line[DISPLAY_LINE_1], "BL TURN   ");       //display information to screen
      display_changed = TRUE;
      
      RIGHT_REVERSE_SPEED = RIGHT_ALIGN;        //set right motor to alignment PWM value
      LEFT_FORWARD_SPEED = LEFT_ALIGN;          //set left motor to alignment PWM value
      check_right_oopsie();                     //safety check for right wheel (forward->reverse change)
      
      if((Left_Detect_Hold > DETECT_THRESHOLD) && (Right_Detect_Hold > DETECT_THRESHOLD)){ //check sampled values, if black line value read by both detectors
        motors_off();                                                                      //stop motors
        //state = IDLE;                                                                           
        
        P6OUT |= LCD_BACKLITE;                                  //enable backlite
        strcpy(display_line[DISPLAY_LINE_1], "BL TRAVEL ");     //display information to screen
        display_changed = TRUE;
      
        sec_5_flag = ON;        //set 5 second wait flag high
        TB2CCTL1 |=  CCIE;      //enables 5 second interrupt timer
        state = ALIGN_DELAY;    //set to next state
      }
      break;
      
    case ALIGN_DELAY:
      if(sec_5_flag == OFF){                    //if 5 seconds has elapsed
        P6OUT &= ~LCD_BACKLITE;                 //disable backlite
        right_speed_val = RIGHT_FORWARD;        //set right motor to forward PWM value
        left_speed_val = LEFT_FORWARD;          //set left motor to forward PWM value
        state = LINE_FOLLOW;                    //set to next state
        
        sec_5_flag = ON;        //set flag high, but do not use 5 sec interrupt yet, used in circle wait process
        TB2CCTL2 |= CCIE;       //enable circle wait interrupt timer, will automatically trigger during line follow process
      }
      break;
      
    case LINE_FOLLOW:
      //Right PID ==============================================================
      //right_error = Right_Detect_Hold - RIGHT_TARGET;
      right_error = right_BL_target - Right_Detect_Hold;     
      
      //P = proportional
      // difference between value have and value wanted
      right_p_term = RIGHT_P_CONST * right_error;
      
      //I - sum of all previous values
      // sum of all previous errors - prevent from almost getting there but not quite there  --- should probably ignore this, just do PD
      //right_i_term = right_i_term + right_error;
      //right_i_term *= RIGHT_I_CONST;
      
      //D - rate of change
      // based on difference between current differences (p) and previous differences - allow to not overshoot, and not over correct
      right_d_term = (Right_Detect_Hold - Right_Detect_Hold_Prev) * RIGHT_D_CONST;
      
      Right_PID = right_p_term + right_i_term - right_d_term;   //combine values to determine PID value
      Right_Detect_Hold_Prev = Right_Detect_Hold;               //record current detect value for next iteration
      //========================================================================
      
      //Left PID ===============================================================
      //left_error = Left_Detect_Hold - LEFT_TARGET;
      left_error = left_BL_target - Left_Detect_Hold;
        
      //P = proportional
      left_p_term = LEFT_P_CONST * left_error;
      
      //I - sum of all previous values
      //left_i_term = left_i_term + left_error;
      //left_i_term *= LEFT_I_CONST;
      
      //D - rate of change
      left_d_term = (Left_Detect_Hold - Left_Detect_Hold_Prev) * LEFT_D_CONST;
      
      Left_PID = left_p_term + left_i_term - left_d_term;       //combine values to determine PID value
      Left_Detect_Hold_Prev = Left_Detect_Hold;                 //record current detect value for next iteration
      //========================================================================
      
      //Check Right and Left Wheel Speed Bounds ================================
      if(right_speed_val + Right_PID > RIGHT_FOLLOW_LIMIT){     //if combined right speed is greater than set PWM limit
        right_speed_val = RIGHT_FOLLOW_LIMIT;                   //set speed to limit
      }
      else if(right_speed_val + Right_PID < WHEEL_OFF){         //if combined right speed is less than 0
        right_speed_val = WHEEL_OFF;                            //set wheel speed to off
      }
      else{
        right_speed_val = right_speed_val + Right_PID;          //otherwise, add PID value to right speed
      }
      
      if(left_speed_val + Left_PID > LEFT_FOLLOW_LIMIT){        //if combined left speed is greater than set PWM limit
        left_speed_val = LEFT_FOLLOW_LIMIT;                     //set speed to limit
      }
      else if(left_speed_val + Left_PID < WHEEL_OFF){           //if combined left speed is less than 0
        left_speed_val = WHEEL_OFF;                             //set wheel speed to off
      }
      else{
        left_speed_val = left_speed_val + Left_PID;             //otherwise, add PID value to left speed
      }
      
      //CATCH IF BOTH DETECTORS ARE ON LINE OR OFF OF LINE, RECORD LAST DIRECTION OF LINE
      if((Left_Detect_Hold > left_BL_target) && (Right_Detect_Hold > right_BL_target)){         //if both detectors are on the black line
        left_speed_val = LEFT_SLOW;                                                             //set both wheels to go straight
        right_speed_val = RIGHT_SLOW;                                                           //
      }
      else if((Left_Detect_Hold < left_BL_target) && (Right_Detect_Hold < right_BL_target)){    //if both detectors are off the black line
        if(last_direction == LEFT){                     //if last direction of line was left of car
          left_speed_val = WHEEL_OFF;                   //turn car left
          right_speed_val = RIGHT_FOLLOW_LIMIT;         //
        }
        else{                                           //if last direction of line was right of car
          left_speed_val = LEFT_FOLLOW_LIMIT;           //turn car right
          right_speed_val = WHEEL_OFF;                  //
        }
      }
      else{                                             //one detector on the black line, one off the black line
        if(Left_Detect_Hold > Right_Detect_Hold){       //if left detector on line
          last_direction = LEFT;                        //black line last to left of car
        }
        else{                           //if right detector on line
          last_direction = RIGHT;       //black line last to right of car
        }
      }
      //========================================================================
      
      RIGHT_FORWARD_SPEED = right_speed_val;    //set wheel speeds to speeds determined above
      LEFT_FORWARD_SPEED = left_speed_val;      //
      check_right_oopsie();     //safety check
      check_left_oopsie();      //safety check
      break;
      
    case END_PROGRAM_START:
      P6OUT |= LCD_BACKLITE;                                    //enable backlite
      strcpy(display_line[DISPLAY_LINE_1], "BL EXIT   ");       //display information to screen
      display_changed = TRUE;
      
      motors_off();             //disable motors
      
      sec_5_flag = ON;          //enable 5 second wait flag
      TB2CCTL1 |=  CCIE;        //enables 5 second interrupt timer
      state = END_PROGRAM_TURN; //set to next state
      break;
      
    case END_PROGRAM_TURN:
      if(sec_5_flag == OFF){                    //if 5 seconds has elapsed
        motors_off();                           //disable motors
        RIGHT_REVERSE_SPEED = RIGHT_ALIGN;      //turn car perpendicular to line
        LEFT_FORWARD_SPEED = LEFT_ALIGN;        //
        check_right_oopsie();                   //safety check for right motor (forward->backward)
        
        sec_flag = ON;                  //enable 1 second timer flag
        TB0CCTL1 |=  CCIE;              //enables 1 second timer interupt
        state = END_PROGRAM_TURN_WAIT;  //set to next state
      }
      break;
      
    case END_PROGRAM_TURN_WAIT:
      if(sec_flag == OFF){              //once 1 second timer has elapsed
        P6OUT &= ~LCD_BACKLITE;         //disable backlite
        motors_off();                   //disable motors
        
        sec_flag = ON;                  //set second wait timer flag high as safety precaution
        TB0CCTL1 |=  CCIE;              //enables 1 second timer interrupt
        state = END_PROGRAM_EXIT;       //set to next state
      }
      break;
      
    case END_PROGRAM_EXIT:
      if(sec_flag == OFF){                      //once 1 second has elapsed
        motors_off();                           //turn motors off as safety precaution
        RIGHT_FORWARD_SPEED = RIGHT_FORWARD;    //move car forward
        LEFT_FORWARD_SPEED  = LEFT_FORWARD;     //
        check_right_oopsie();   //safety check
        check_left_oopsie();    //safety check
      
        sec_5_flag = ON;                //enable 5 second timer flag
        TB2CCTL1 |=  CCIE;              //enables 5 second timer interrupt
        state = END_PROGRAM_FINISH;     //set to next state
        
        P6OUT |= LCD_BACKLITE;  //
        P6OUT |= GRN_LED;       //show off for dramatic ending
        P1OUT &= ~RED_LED;      //
        sec_flag = ON;          //enable 1 second wait flag
        TB0CCTL1 |=  CCIE;      //enable 1 second wait timer interrupt
      }
      break;
      
    case END_PROGRAM_FINISH:
      if(sec_5_flag == OFF){                                    //once 5 seconds has elapsed
        strcpy(display_line[DISPLAY_LINE_1], "BL STOP   ");     //
        strcpy(display_line[DISPLAY_LINE_2], "I'm.      ");     //display information to screen
        strcpy(display_line[DISPLAY_LINE_3], "Done.     ");     //
        display_changed = TRUE;
        timer_flag = OFF;               //stop on-screen timer
        motors_off();                   //stop motors
        
        if(sec_flag == OFF){            //once second has elapsed
          P6OUT ^= LCD_BACKLITE;        //on/off backlite
          P1OUT ^= RED_LED;             //on/off red LEDs
          P6OUT ^= GRN_LED;             //on/off green LEDs
          sec_flag = ON;                //set 1 sec flag high for next swap
          TB0CCTL1 |=  CCIE;            //enable 1 second timer interrupt
        }
      }
      break;
  }
}

