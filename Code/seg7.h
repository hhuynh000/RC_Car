/**
 *   @file seg7.h
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Declaration of functions to display digits on the 7 segments display.
 */

#ifndef seg7_h
#define seg7_h

#include "utils.h"

#define PORTC _SFR_MEM8(0x28)
#define DDRC _SFR_MEM8(0x27)

/**
 * @brief An array of 8-bit values that correspond to digits 0-9 on the 7 segments display.
 */
extern uint8_t seven_seg_digits[10];

/**
 * @brief Setup DDRL/DDRC registers to display digits on the 7 segment display.
 * 
 * Set DDRL most significant 4 bits, which configure pins 42-45 to be output pin.
 * Pins 42-45 controls Dig1 - Dig4 respectively. 
 * Set all bits in DDRC, which configure pins 30-37 to be output pin. Where pin 30 is DP,
 * and pins 37-31 is segment A-G respectively.
 */
void setupSeg7();

/**
 * @brief Display a given digit (0-9).
 * 
 * Set PORTC to turn on the corresponding segments to
 * represent a given digit 0-9. Does not target any specific digit.
 * 
 * @param digit A number between 0 and 9 representing on digit place value.
 */
void displayDigit(int digit);

/**
 * @brief Compute each digit of a given number.
 * 
 * Only support up to four digit (max 9999), any value larger
 * will get overflow back to 0. For example, if the given number is 10001
 * the coverted digits is x x x 2.
 * 
 * @param number The given number to be converted into its digits
 * @param arrays A pass in array to store the resulting digits.
 * @return A pointer to an array of the corresponding digits. 
 * The array order is from the right most digit to the left most digit.
 * If a number does not have a specific digit, the value is set to -1.
 */
void convertDigit(int number, int *arrays);

/**
 * @brief Display a desired segment corresponding to a 8-bit value.
 * 
 * 
 * @param number An 8-bit value correspond to segments on display.
 */
void displaySegment(uint8_t segments);

#endif // seg7_h