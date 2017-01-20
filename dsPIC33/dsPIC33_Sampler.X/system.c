/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
#include <xc.h>
#elif defined(__C30__)
#if defined(__dsPIC33E__)
#include <p33Exxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif
#endif


#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "system.h"          /* variables/params used by system.c             */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c.                                          */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.*/

/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void) {
    /* Disable Watch Dog Timer */
    RCONbits.SWDTEN = 0;

#if 1
    // 70MHz..Configure PLL prescaler, PLL postscaler, PLL divisor.
    PLLFBD = 63; // M=65
    CLKDIVbits.PLLPOST = 0;     // N2=2
    CLKDIVbits.PLLPRE = 0;      // N1=3

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);
#else
    // 40MHz..Configure PLL prescaler, PLL postscaler, PLL divisor.
    // FRM_ADC Ex 16-3
    PLLFBD = 41;                // M = 43 for 7.37MHz internal RC
    CLKDIVbits.PLLPOST = 0;     // N1 = 2
    CLKDIVbits.PLLPRE = 0;      // N2 = 2
    OSCTUN = 0;

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0x01);
#endif
    /* Wait for PLL to lock */
    while(OSCCONbits.LOCK == 0);
}

void Delay_us(unsigned int delay){
    unsigned int i;

    for (i = 0; i < delay; i++){
        __asm__ volatile ("repeat #39");
        __asm__ volatile ("nop");
    }
}
