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
#include <stdio.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include "serial.h"        // Serial support
#include "audio.h"         // audio support.
void test001();
void test002(USER_ADC_CONF conf);

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */
uint16_t ADCValue;  // ADC 12 bit data.
volatile char ADC[2048];    // ADC 10 bit data. Modified in interrupt.
volatile int adcCount = 0;      // FULL if => 2047. Use as index into ADC.

/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/

int16_t main(void) {
//    USER_ADC_CONF conf = USER_ONE_CHAN;
    USER_ADC_CONF conf = USER_FOUR_CHAN;

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp(conf);          // ADC system
    ConfigureUART();        // UART
    ConfigurePWM();         // PWM

    INTCON2bits.GIE = 1;        // Enable Interrupts
//    INTCON2bits.GIE = 0;        // Disable Interrupts
    
    
    while (1) {
//        test001();          // never returns.
        test002(USER_FOUR_CHAN);
    }
}

/* Archive old test code here. */
// ADC tests.. pass in configuration
void test002(USER_ADC_CONF conf)
{
    char strOut[32];
//    bool tog = false;
    int strLen;

    AD1CON1bits.SAMP = 1; // Start sampling
    IFS0bits.AD1IF = 0;  //After conversion, ADxIF is set to 1 and must be cleared.

    initAudioDDS();     // Play preset tune once. (see audio.c)
    
//    strLen = sprintf(strOut, "Start\n");
    strLen = sprintf(strOut, "Format: Ch0 Ch1 Ch2 Ch3\n");
    writeBuff(strOut, strLen);
    
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
//            while (!_AD1IF);             // Wait for the all conversion to complete
//            _AD1IF = 0;                 // Clear conversion done bit.
//                setLed(true);                              // TIMING TEST

            // Dump buffer to serial.
              if( adcCount == sizeof(ADC)) {
//                strLen = sprintf(strOut, "%03u %03u %03u %03u \n", (uint8_t)ADC[0],(uint8_t)ADC[1],(uint8_t)ADC[2],(uint8_t)ADC[3]);
                strLen = sprintf(strOut, "%02x %02x %02x %02x \n", (uint8_t)ADC[0],(uint8_t)ADC[1],(uint8_t)ADC[2],(uint8_t)ADC[3]);
                writeBuff(strOut, strLen);
                adcCount = 0;

                // Time sample rate.
                // 5us -> 200KHz per channel x4 channels -> 800KHz total rate.
//                if (tog) {
//                    setLed(true);
//                } else {
//                    setLed(false);
//                }
//                tog = !tog;
              }

#endif
//            ADC[0] = ADC1BUF0; // Read the ADC conversion result
//            ADC[1] = ADC1BUF1; // Read the ADC conversion result
//            ADC[2] = ADC1BUF2; // Read the ADC conversion result
//            ADC[3] = ADC1BUF3; // Read the ADC conversion result

//            if ( (ADC[0] < 0x0200) ||
//                    (ADC[1] < 0x0200) ||
//                    (ADC[2] < 0x0200) ||
//                    (ADC[3] < 0x0200) ) {
//                setLed(false);
//            } else {
//                setLed(true);
//            }

        }
//                setLed(false);                              // TIMING TEST
    }
}

// Simple I/O tests
void test001() {
    uint16_t count = 0;
    uint16_t loops = 0;
    
    char buff[5] = {'A', 'B', 'C','D','\n'};

    while (1) {
        if (count == 0xFFF0) {
            count = 0;
            if (loops == 1) {
                setLed(false);
            }
            if( loops == 90) {
                setLed(true);
//                writeChar('A');             // Output about every 19.7 ms.
                writeBuff(buff, 5);
            }
            if( loops == 100) {
                loops = 0;
            }                
            ++loops;
        }
        ++count;
    }
}
