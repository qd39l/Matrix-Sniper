//=========================================================================
// adc.c
//=========================================================================
// ECE 3140 Final Project
// Yang Shen and Owen Deng
//
// ADC control library.

#include "adc.h"
#include "fsl_common.h"


//------------------------------------------------------------------------
// adc_init()
//------------------------------------------------------------------------
// initializes ADC1 and ADC0
// ADCs set up with 16 bit precision. Normal mode. Single pin mode. Input.

void adc_init(void){
  // SCGC6 bit 27 - ADC0
  SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // enable the ADC clock
  
  ADC0->CFG1 |= ADC_CFG1_MODE(3); // 16bit resolution
  ADC0->SC1[0] |= ADC_SC1_ADCH(31); // DP0 as input
  
  // SCGC3 bit 27 - ADC1
  SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK; // enable the ADC clock
  ADC1->CFG1 |= ADC_CFG1_MODE(3); // 16bit resolution
  ADC1->SC1[0] |= ADC_SC1_ADCH(31); // DP0 as input  
}


//------------------------------------------------------------------------
// adc_read()
//------------------------------------------------------------------------
// read from adc1 or adc0. Read from DP0.

int adc_read(int num){
  
  if (num == 0){
    ADC0->SC1[0] = 0 & ADC_SC1_ADCH_MASK; // write to SC1A to start conversion, read 0
    while(ADC0->SC2 & ADC_SC2_ADACT_MASK); // conversion in progress
    while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)); // wait until conversion complete
    return (int)ADC0->R[0];
  }
  
  ADC1->SC1[0] = 0 & ADC_SC1_ADCH_MASK; // write to SC1A to start conversion, read 0
  while(ADC1->SC2 & ADC_SC2_ADACT_MASK); // conversion in progress
  while(!(ADC1->SC1[0] & ADC_SC1_COCO_MASK)); // wait until conversion complete
  return (int)ADC1->R[0];
}
