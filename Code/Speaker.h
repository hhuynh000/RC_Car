/**
 *   @file Speaker.h
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Declaration of functions to interface with a speaker.
 */

#ifndef Speaker_h
#define Speaker_h

#include "utils.h"

/**
 * @brief TCCR4A Macros for accessing Timer/Counter 4A control register A.
 */
#define TCCR4A _SFR_MEM8(0xA0)

/**
 * @brief TCCR4B Macros for accessing Timer/Counter 4A control register B.
 */
#define TCCR4B _SFR_MEM8(0xA1) // Timer/Counter 4A control reg B

/**
 * @brief DDRH Macros for accessing DDRH register
 */
#define DDRH _SFR_MEM8(0x101)

/**
 * @brief OCR4A Macros for accessing Timer/Counter 4A compare value register.
 */
#define OCR4A _SFR_MEM16(0xA8) // Output compare register (OCR4A)

// PORTH/DDRH[6:3] = pins 6-9 respectively
// TCCR4B[2:0] = CS4[2-0] (clock select)
// TCCR4A[7:6] = COM4A1-COM4A0 (compare mode)
// TCCR4A[1:0] = WGM40-WGM41
// TCCR4B[3] = WGM42

/**
 * @brief Setup timer4A to generate sound
 * 
 * Set pin 6 to output mode (DDRH). Set timer4A to CTC mode with a prescaler of 1.
 * Inititally disable compare mode and set compare value to 0.
 */ 
void setupSpeaker();

/**
 * @brief Create a square wave with specified frequency to generate a tone.
 * 
 * The tone is played continously using pin 6. Due to 16 bit int value on Atemega2560, 
 * the lowest possible frequency is 122Hz. Default to disabling the compare mode.
 * @param freq Frequency of generated tone (int)
 */
void createTone(int freq);

/**
 * @brief Stop playing the current tone.
 * 
 */
void stopTone();

#endif // Speaker_h