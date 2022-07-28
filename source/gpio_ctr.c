//=========================================================================
// gpio_ctr.c
//=========================================================================
// ECE 3140 Final Project
// Yang Shen and Owen Deng
//
// GPIO control library

#include "gpio_ctr.h"


//------------------------------------------------------------------------
// GPIO_init()
//------------------------------------------------------------------------
// configure D0 and C4 as output GPIO. Initially set to low.

void GPIO_init(void){
  // PORT D0
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
  
  // set GPIO
  PORTD->PCR[0] = PORT_PCR_MUX(001);
  
  // enable output
  GPIOD->PDDR |= (1<<0);
  
  // clear all the bits. 0 is off. 1 is on.
  GPIOD->PDOR = 0;
  
  // PORT C4
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  
  // set GPIO
  PORTC->PCR[4] = PORT_PCR_MUX(001);
  
  // enable output
  GPIOC->PDDR |= (1<<4);
  
  // cleear all the bits. 0 is off. 1 is on.
  GPIOC->PDOR = 0;
}


//------------------------------------------------------------------------
// GPIO_off()
//------------------------------------------------------------------------
// Drive low on the corresponding GPIO. num 1 -> C4. num 0 -> D0.

void GPIO_off(int num){
  if (num == 0){
    GPIOD->PCOR = 1; // clear D0
  }
  else{
    GPIOC->PCOR = (1<<4); // clear C4
  }
}


//------------------------------------------------------------------------
// GPIO_on()
//------------------------------------------------------------------------
// Drive high on the corresponding GPIO. num 1 -> C4. num 0 -> D0.

void GPIO_on(int num){
  if (num == 0){
    GPIOD->PSOR = 1; // set D0
  }
  else{
    GPIOC->PSOR = (1<<4); // set C4
  }
}
