//=========================================================================
// sevenseg.h
//=========================================================================
// ECE 3140 Final Project
// Yang Shen and Owen Deng
// 
// seven-segment display control library

#include "3140_i2c.h"

#ifndef __sevenseg__
#define __sevenseg__

int sevenseg_init(void);
int sevenseg_blinkrate(uint8_t b);
int sevenseg_setBrightness(uint8_t b);
void sevenseg_writeDigit(uint8_t d, int num);
void sevenseg_clear(void);
void sevenseg_writeTarget(int target);

#endif
