/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Nels D. "Chip" Pearson (aka CmdrZin)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * serial.c
 * origin:  09sept17    0.01    ndp
 * 
 * Serial support functions for dsPIC33EP32MC202
 * ref: ..\Downloads\Microchip\dsPIC33 Family Reference Manual\S17.pdf
 * 
 */

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

#include "serial.h"

/*
 * Tx out one character. Test Empty/Full flag before sending.
 * NOTE: BLOCKING call.
 */
void writeChar(char val)
{
    while( U1STAbits.UTXBF );
    U1TXREG = val;              // Send when U1TXREG becomes empty.
}

/*
 * Tx out all characters in buffer. Test FIFO FULL flag before filling.
 * NOTE: BLOCKING call until all character have been sent to the FIFO.
 */
void writeBuff(char *buff, int val)
{
    while(val != 0) {
        while( !U1STAbits.UTXBF ) {
            U1TXREG = *buff++;              // Send if FIFO is NOT full.
            --val;
        }
    }
}


/*     INTERRUPT SUPPORT    */
/*
 * RX Interrupt Service
 */
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    char dump;
    dump = U1RXREG;          // Get one character. TODO - CHANGE THIS.

    IFS0bits.U1RXIF = 0;     // Clear RX Interrupt flag
}

/*
 * TX Interrupt Service
 */
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;    // Clear TX Interrupt flag..S17.pdf Sect 5.2
}

