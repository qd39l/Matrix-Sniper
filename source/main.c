//=========================================================================
// main.c
//=========================================================================
// ECE 3140 Final Project
// Yang Shen and Owen Deng
// 
// Wow! The Matrix Sniper Beta (WTMSB)
// This is a laser shooting game with two targets. A seven segment display 
// shows current scores and failures. +1 point for shooting at correct
// target. -1 point and +1 failure for missing the target or shooting at
// wrong target. t_score and t_failures determine the winning and losing
// requirements.

#include "fsl_common.h"
#include "utils.h"
#include "3140_serial.h"
#include "3140_i2c.h"
#include "sevenseg.h"
#include "adc.h"
#include "gpio_ctr.h"
#include <stdio.h>
#include <stdlib.h>

int current_score = 0;
int failures = 0;
int t_score = 5; // player wins when current_score == t_score
int t_failures = 2; // player loses when failures == t_failures


//------------------------------------------------------------------------
// print_num()
//------------------------------------------------------------------------
// print via serial USB to computer for debugging purposes

void print_num(int num){
  char str[80];
    
  sprintf(str, "Value of number = %d\n\r", num);
    
  uart_putString(str);
}


//------------------------------------------------------------------------
// rand_btw()
//------------------------------------------------------------------------
// generate a random number between low to 9 * (high - low) + low

int rand_btw(int low, int high){
  float rand_num = rand() % 10; // integer between 0 and 9
  rand_num = rand_num * (high - low); // variance set
  rand_num = rand_num + low; // bias set
  return (int)rand_num;
}


//------------------------------------------------------------------------
// s_delay()
//------------------------------------------------------------------------
// short delay

void s_delay(void){
	int j;
	for(j=0; j<500000; j++); // approx. 
}


//------------------------------------------------------------------------
// read_and_detect()
//------------------------------------------------------------------------
// ADC reading from the two targets for "interval" times. return 1 if 
// target is correct. return 2 if target is incorrect. return 0 if threhold
// not met (no target detected).

int read_and_detect(int interval, int target){
  int volt0;
  int volt1;
  
  for (int i = 0; i < interval; i ++){
    // check target 0
    volt0 = adc_read(0);
    if ((volt0 > 9000)){
      if (target == 0){
        return 1;
      }
      else{
        return 2;
      }
    }
    
    // check target 1
    volt1 = adc_read(1);
    if ((volt1 > 6000)){
      if (target == 1){
        return 1;
      }
      else{
        return 2;
      }
    }
  }
  return 0;
}


//------------------------------------------------------------------------
// trivial_read()
//------------------------------------------------------------------------
// A trivial function that mirrors read_and_detect(). Aim to run the same
// CPU cycles as read_and_detect() so that the LEDs are on and off for
// the same amount of time. Always return 0.

int trivial_read(int interval){
  int volt0;
  int volt1;
  int target = 0;
  
  for (int i = 0; i < interval; i ++){
    // check target 0
    volt0 = adc_read(0);
    if ((volt0 > 9999999)){
      if (target == 0){
        return 0;
      }
      else{
        return 0;
      }
    }
    
    // check target 1
    volt1 = adc_read(1);
    if ((volt1 > 9999999)){
      if (target == 1){
        return 0;
      }
      else{
        return 0;
      }
    }
  }
  return 0;
}


//------------------------------------------------------------------------
// detect()
//------------------------------------------------------------------------
// detect the target. return 1 if target is correct. return 0 if target is
// incorrect or time has passed without receiving laser signal.

int detect(int read_times, int target){
  for (int i = 0; i < 20; i ++){
    GPIO_on(0);
    GPIO_on(1);

    for (int i = 0; i < 9000; i ++);
    
    if (trivial_read(read_times)){ // this branch never taken
      return 1;
    }
    
    GPIO_off(0);
    GPIO_off(1);
    for (int i = 0; i < 9000; i ++);
    
    int flag = read_and_detect(read_times, target);
    
    if (flag == 1){ // correct target
      return 1;
    }
    else if (flag == 2){
      return 0;
    }
  }

  return 0;
}


//------------------------------------------------------------------------
// display_update()
//------------------------------------------------------------------------
// update the display score

void display_update(void){
  sevenseg_clear();
  sevenseg_writeDigit(0, current_score);
  sevenseg_writeDigit(3, failures);
}


//------------------------------------------------------------------------
// b_lose()
//------------------------------------------------------------------------
// called when player has lose the game. only display failures and blink

void b_lose(void){
  sevenseg_blinkrate(2);
  sevenseg_clear();
  sevenseg_writeDigit(3, failures);
}


//------------------------------------------------------------------------
// b_win()
//------------------------------------------------------------------------
// called when player has won the game. only display scores and blink

void b_win(void){
  sevenseg_blinkrate(2);
  sevenseg_clear();
  sevenseg_writeDigit(0, current_score);
}


//------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------

int main (void){

  // initialize all hardwares
  I2C_ReleaseBus();
  I2C_Init();
  LED_Initialize();
  adc_init();
  uart_init();
  GPIO_init();
  if (sevenseg_init() != 1){
    return -1;
  }
  sevenseg_clear(); // clear the seven-segment display
  display_update(); // show current scores
  
  // wait for the player to be ready. LEDs ON.
  GPIO_on(0);
  GPIO_on(1);
  for (int i = 0; i < 30000000; i ++); // approx. 6s
  GPIO_off(0);
  GPIO_off(1);
  
  for(int j=0; j<15000000; j++); // approx. 4s
  
  // blink LEDs to signal the start of the game
  sevenseg_blinkrate(1);
  for (int i = 0; i < 7; i++){
    GPIO_on(0);
    GPIO_on(1);
    s_delay();
    GPIO_off(0);
    GPIO_off(1);
    s_delay();
  }
  sevenseg_blinkrate(0);
  
  // seed srand from reading adc0
  srand(adc_read(0)); 
  
  // start gameplay!
  while ((current_score != t_score) && (failures != t_failures)){
    // generate random number for delay
    int rand_time = rand_btw(10,20);
    
    // random delay
    for (int i = 0; i < rand_time; i ++){
      s_delay();
    }
    
    // decide a random target
    int target = rand() % 2; // random number 1 or 0
    
    // display target
    sevenseg_writeTarget(target);
    
    // LED flashes and detect
    // correct target - increment current score
    if (detect(12500, target)){
      current_score++;
      //failures = 0;
    }
    // incorrect target or time-out - decrement current score & increment failures
    else{
      if (current_score!=0){
        current_score--;
      }
      failures ++;
    }
    
    // update score
    display_update();
  }
  
  // game ends
  // if lose, blink failures
  if (failures == t_failures){
    b_lose();
  }
  
  // if win, blink current score
  else{
    b_win();
  }
  
  // turn on both panels and on-board LED
  GPIO_on(0);
  GPIO_on(1);
  LEDRed_On();
  
  for (unsigned int i = 0; i < 3000000000; i ++);

  return 0;
}
