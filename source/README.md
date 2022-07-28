# Matrix Sniper

This is the source code for ECE3140 final project by Owen Deng (qd39) and Yang Shen (ys656). It is the implementation of a LED shooting arcade game with the FRDM-K64F board. Detailed information regarding the project can be found on our project GitHub page: https://qd39l.github.io/Matrix-Sniper/

## How to run

First, you need to set up a Keil uVision project appropriately following the guidelines from ECE3140 SP20 canvas I2C tutorial. Specifically, this involves including `fsl_clock.c`, `fsl_common.c`, `fsl_gpio.c`, and `fsl_i2c.c` in your build environment, and expanding the heap size.

Second, you need to connect the circuit appropriately following the guidelines from our GitHub page. It involves using the same pins of the board described. Specific details involved in the wiring of the LED panel is also described there. For wiring the 7-Segment display, look at the tutorial from Adafruit here: https://learn.adafruit.com/adafruit-led-backpack/1-2-inch-7-segment-backpack

Lastly, include all the files in this library in your project, compile and load it on the board. If you set up everything correctly, the game will run. The `main.c`· file contains the main function of the program, and other files provide necessary functions for interacting with the hardwares.

## Hardware components

36 Ultrabright Red LEDs


One 7-segment display from Adafruit


One 5V power source


Several jumper wires


Resistors (38 * ~470 Ohms, 6 * 220 Ohms, 2 * 1 MOhms)


Breadboard


Green laser pen


Two protoboards


Two 2N5551 NPN BJTs.

## Code Structure

The `main.c`· file contains the major part of the game from beginning to end. All the other files either provide helper functions (`utils`) or provide functions that help us interact with the hardwares (`3140_i2c`, `3140_serial`, `adc`, `gpio_ctr`, `sevenseg`). Files including `fsl_clock.c`, `fsl_common.c`, `fsl_gpio.c`, and `fsl_i2c.c` are essential in our code to use the i2c and the gpio modules on the FRDM-K64F board.

## Additional information about the game

This section is also described in our GitHub page.

After turning on the power supply, the LED panels would light up, and the 7-segment display would show the current score, which should be “0  0.” The number on the left shows the number of failures, and the number on the right shows the number of points. After ~10 seconds of waiting time for the player to get ready, the LED panels and the 7-segment display would flash for 3 seconds to signal the beginning of the game. At this point, the LED panels are turned off. After a random delay time, the board randomly picks one of the targets, the LED panels flash, and the 7-segment display would show “ [ [ [ [ (left target)” of “] ] ] ] (right target).” If the player shoots the correct LED panel within a given amount of time, 1 point is scored. Otherwise, if the player shoots at the incorrect target or fails to shoot at the target within the given time, the player will lose 1 point and get 1 failure. After each round, the 7-segment display updates the current score. 

The game ends when the player wins the game by scoring 5 points or making 2 failures. When the game ends, both LEDs are turned on. If the player wins, the number on the left (showing failures) would flash “2”. Otherwise, the number on the right (showing points) would flash “5”.

