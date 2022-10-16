/**
 *   @file utils.h
 *   @author Huy and Richard
 *   @date 16-May-2022
 *   @brief Declaration of useful utilities functions
 * 
 *   Contains basic bitwise operations: set/clear/complement bits
 */

#ifndef utils_h
#define utils_h

#include "Arduino.h"

/**
 * @brief DRRB Macros
 */
#define DDRB _SFR_MEM8(0x24)

/**
 * @brief PORTB Macros 
 */
#define PORTB _SFR_MEM8(0x25)

/**
 * \def TCCR0A Macros for accessing Timer/Counter 0 Register A.
 */
#define TCCR0A _SFR_MEM8(0x44)
// TCCR0A[7:6] - COM0A1-COM0A0 control compare output A mode

/**
 * @brief TCCR0B Macros for accessing Timer/Counter 0 Register B.
 */
#define TCCR0B _SFR_MEM8(0x45)
// TCCR0B[2:0] - prescaler select
// TCCR0B[3] - CTC mode

/**
 * @brief OCR0A Macros for accessing output compare register A.
 */
#define OCR0A _SFR_MEM8(0x47)

/**
 * @brief TIMSK0 Macros for accessing timer/counter interrupt mask regisiter.
 */
#define TIMSK0 _SFR_MEM8(0x6E)
// TIMSK0[1] - enable interrupt for output compare match A

/**
 * @brief PORTL Macros for accessing PORTHL registers (pins 42-49).
 */
#define PORTL _SFR_MEM8(0x10B) // pin 49-42 port L

/**
 * @brief DDRL Macros for accessing DDRL registers (pins 42-49).
 */
#define DDRL _SFR_MEM8(0x10A)

/**
 * @brief PORTC Macros for accessing PORTC registers (pins 30-37).
 */
#define PORTC _SFR_MEM8(0x28)

/**
 * @brief DDRC Macros for accessing DDRC registers (pins 30-37).
 */
#define DDRC _SFR_MEM8(0x27)

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

/**
 * @brief ICR4 Macros for accessing Timer/Counter 4 input capture register
 */
#define ICR4 _SFR_MEM16(0xA6)

/**
 * @brief PCMSK0 Macros for accessing pin change mask register 0
 */
#define PCMSK0 _SFR_MEM8(0x6B)

/**
 * @brief PCMSK2 Macros for accessing pin change mask register 1
 */
#define PCMSK2 _SFR_MEM8(0x6D)
/**
 * @brief PCICR Macros for accessing pin change control register
 */
#define PCICR _SFR_MEM8(0x68)

/**
 * @brief Define a function pointer.
*/
typedef void (*functionPtr)();

/**
 * @brief Start a function given a pointer to that function
 * 
 * @param function A pointer to a function.
 */
void startFunction (functionPtr function);

/**
 * @brief Set specified bits of the given input (turn on bit)
 * @param input A 16 bits input value
 * @param mask A 16 bits sequence indicating which bit to set, ex. 0b00..01 will turn on the first bit
 * @return The function returns the resulting 16 bits output of the bitwise operation
*/
uint16_t setBit(uint16_t input, uint16_t mask);

/**
 * @brief Clear specified bits of the given input (turn off bit)
 * @param input A 16 bits input value
 * @param mask A 16 bits sequence indicating which bit to clear, ex. 0b00..01 will turn off the first bit
 * @return The function returns the resulting 16 bits output of the bitwise operation
*/
uint16_t clearBit(uint16_t input, uint16_t mask);


/**
 * @brief Set and clear specified bits of the given input
 * 
 * Mask for set and clear can have overlapping bit, where 
 * the set operation is done first then the clear operation
 * @param input A 16 bits input value
 * @param maskSet A 16 bits sequence indicating which bit to set, ex. 0b00..01 will turn on the first bit
 * @param maskClear A 16 bits sequence indicating which bit to clear, ex. 0b00..01 will turn off the first bit
 * @return The function returns the resulting 16 bits output of the bitwise operation
*/
uint16_t setClearBit(uint16_t input, uint16_t maskSet, uint16_t maskClear);

/**
 * @brief Complement specified bits of the given input (flip the bit)
 * @param input A 16 bits input value
 * @param mask A 16 bits sequence indicating which bit to complement, ex. 0b00..01 will complement the first bit
 * @return The function returns the resulting 16 bits output of the bitwise operation
*/
uint16_t compBit(uint16_t input, uint16_t mask);

#endif // utils_h