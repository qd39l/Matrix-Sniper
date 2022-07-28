//=========================================================================
// sevenseg.c
//=========================================================================
// ECE 3140 Final Project
// Yang Shen and Owen Deng
// 
// seven segment display control library.
// referenced Adafruit LEDBackpack library

#include "sevenseg.h"
 
#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3
#define HT16K33_CMD_BRIGHTNESS 0xE0

#define SEVENSEG_ADDR 0x70

static const uint8_t numbertable[] = {
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F, /* 9 */
	0x77, /* a */
	0x7C, /* b */
	0x39, /* C */
	0x5E, /* d */
	0x79, /* E */
	0x71, /* F */
};


//------------------------------------------------------------------------
// sevenseg_init()
//------------------------------------------------------------------------
// initialize the sevensegment display.

int sevenseg_init(void){
  // turn on oscillator
  if (I2C_WriteReg(SEVENSEG_ADDR, 0x21) != 1)
  {
    return -1;
  }
  
  // no blink
  if (sevenseg_blinkrate(HT16K33_BLINK_OFF) != 1)
  {
    return -1;
  }
  
  // set brightness
  if (sevenseg_setBrightness(2) != 1)
  {
    return -1;
  }
  
  return 1;
}


//------------------------------------------------------------------------
// sevenseg_blinkrate()
//------------------------------------------------------------------------

int sevenseg_blinkrate(uint8_t b){
  if (b > 3) b = 0; // turn off if not sure
  
  if (I2C_WriteReg(SEVENSEG_ADDR, HT16K33_BLINK_CMD|HT16K33_BLINK_DISPLAYON|(b<<1)) != 1)
  {
    return -1;
  }
  return 1;
}


//------------------------------------------------------------------------
// sevenseg_setBrightness()
//------------------------------------------------------------------------
// maximum brightness 15

int sevenseg_setBrightness(uint8_t b) {
  if (b > 15) b = 15;
  
  if (I2C_WriteReg(SEVENSEG_ADDR, HT16K33_CMD_BRIGHTNESS | b) != 1)
  {
    return -1;
  }
  return 1;
}


//------------------------------------------------------------------------
// sevenseg_writeDigit()
//------------------------------------------------------------------------
// write digit d (from right) with number num

void sevenseg_writeDigit(uint8_t d, int num) {
  // 0x00 - leftmost digit
  // 0x02 - 2nd leftmost digit
  // 0x06 - 2nd rightmost digit
  // 0x08 - rightmost digit
  
  uint8_t reg;
  if (d == 3) reg = 0x00;
  else if (d == 2) reg = 0x02;
  else if (d == 1) reg = 0x06;
  else if (d == 0) reg = 0x08;

  I2C_Write2Reg(SEVENSEG_ADDR, reg, numbertable[num]|(0b10000000));
}


//------------------------------------------------------------------------
// sevenseg_writeTarget()
//------------------------------------------------------------------------
// display indicator for target to shoot at

void sevenseg_writeTarget(int target){
  if (!     target){ // target 1
    sevenseg_clear();
    I2C_Write2Reg(SEVENSEG_ADDR, 0x00, 0b00111001|(0b10000000));
    I2C_Write2Reg(SEVENSEG_ADDR, 0x02, 0b00111001|(0b10000000));
    I2C_Write2Reg(SEVENSEG_ADDR, 0x06, 0b00111001|(0b10000000));
    I2C_Write2Reg(SEVENSEG_ADDR, 0x08, 0b00111001|(0b10000000));
  }
  else{ // target 0
    sevenseg_clear();
    I2C_Write2Reg(SEVENSEG_ADDR, 0x00, 0b00001111|(0b10000000));
    I2C_Write2Reg(SEVENSEG_ADDR, 0x02, 0b00001111|(0b10000000)); 
    I2C_Write2Reg(SEVENSEG_ADDR, 0x06, 0b00001111|(0b10000000));
    I2C_Write2Reg(SEVENSEG_ADDR, 0x08, 0b00001111|(0b10000000));
  }
}


//------------------------------------------------------------------------
// sevenseg_clear()
//------------------------------------------------------------------------
// clear all segments on the display panel

void sevenseg_clear(void) {
  for (int d = 0; d <= 4; d ++){
    
    uint8_t reg;
    if (d == 3) reg = 0x00;
    else if (d == 2) reg = 0x02;
    else if (d == 1) reg = 0x06;
    else if (d == 0) reg = 0x08;
    else if (d == 4) reg = 0x04;

    I2C_Write2Reg(SEVENSEG_ADDR, reg, 0x00|(0b10000000));  
  }
}
