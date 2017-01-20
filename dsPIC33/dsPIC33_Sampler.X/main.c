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
            AD1CON1bits.SAMP = 1; // Start sampling
//                setLed(true);                              // TIMING TEST
            Delay_us(1); // Wait for sampling time (1 us)
            AD1CON1bits.SAMP = 0; // Start the conversion
//                setLed(false);                              // TIMING TEST
            while (!AD1CON1bits.DONE); // Wait for the conversion to complete
//                setLed(true);                              // TIMING TEST
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
