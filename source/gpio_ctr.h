//=========================================================================
// gpio_ctr.h
//=========================================================================
// ECE 3140 Final Project
// Yang Shen and Owen Deng
//
// GPIO control library

#include "fsl_common.h"

#ifndef __gpioctr__
#define __gpioctr__

void GPIO_init(void);
void GPIO_off(int num);
void GPIO_on(int num);

#endif
