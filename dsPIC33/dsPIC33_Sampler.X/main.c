/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Nels D. "Chip" Pearson (aka CmdrZin)
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
 */

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


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */
uint16_t ADCValue;  // ADC 12 bit data.
uint16_t ADC[4];    // ADC 10 bit data.

/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/

int16_t main(void) {
//    USER_ADC_CONF conf = USER_ONE_CHAN;
    USER_ADC_CONF conf = USER_FOUR_CHAN;

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp(conf);

    while (1) {
        if (conf == USER_ONE_CHAN) {
            AD1CON1bits.SAMP = 1; // Start sampling
            Delay_us(10); // Wait for sampling time (10 us)
            AD1CON1bits.SAMP = 0; // Start the conversion
            while (!AD1CON1bits.DONE); // Wait for the conversion to complete
            ADCValue = ADC1BUF0; // Read the ADC conversion result

            if (ADCValue < 0x0800) {
                setLed(false);
            } else {
                setLed(true);
            }
        } else {
            // FOUR channel sample
#if 0
            // Manual sample and convert. [see also user.c:InitApp()]
            AD1CON1bits.SAMP = 1; // Start sampling
            Delay_us(1); // Wait for sampling time (1 us)
                setLed(true);                              // TIMING TEST
            AD1CON1bits.SAMP = 0; // Start the conversion
            while (!AD1CON1bits.DONE); // Wait for the conversion to complete
                setLed(false);                              // TIMING TEST
//                setLed(true);                              // TIMING TEST
#else
            // Auto sample and convert. [see also user.c:InitApp()]
//                setLed(false);                              // TIMING TEST
            while (!_AD1IF);             // Wait for the all conversion to complete
            _AD1IF = 0;                 // Clear conversion done bit.
//                setLed(true);                              // TIMING TEST
#endif
            ADC[0] = ADC1BUF0; // Read the ADC conversion result
            ADC[1] = ADC1BUF1; // Read the ADC conversion result
            ADC[2] = ADC1BUF2; // Read the ADC conversion result
            ADC[3] = ADC1BUF3; // Read the ADC conversion result

            if ( (ADC[0] < 0x0200) ||
                    (ADC[1] < 0x0200) ||
                    (ADC[2] < 0x0200) ||
                    (ADC[3] < 0x0200) ) {
                setLed(false);
            } else {
                setLed(true);
            }
        }
//                setLed(false);                              // TIMING TEST
    }
}

/* Archive old test code here. */
void test001() {
    uint16_t count = 0;
#if 1
    while (1) {
        if (count == 0) {
            setLed(false);
        }
        if (count == 32768) {
            setLed(true);
        }
        ++count;
    }
#else
    while (1) {
        asm("nop");
        LATB |= (1 << 4); // set HIGH
        asm("nop");
        asm("nop");
        asm("nop");
        LATB &= ~(1 << 4); // set LOW
    }
#endif
}
