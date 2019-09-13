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

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */
#include "system.h"         // access Delay_us()

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(USER_ADC_CONF config) {
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISB = 0xBFEF;         // RB14,RB4 as output for LED D4.

    /* Setup analog functionality and port direction */
    //AN PortB as digital I/O
    ANSELB = 0x0000;

    /* Initialize peripherals */

    if (config == USER_ONE_CHAN) {
        /* see also dsPIC33_FRM_ADC Section 16 Example 16-1 code. Example based on Fosc = 80MHz */
        ANSELA = ANSELB = 0x0000;
        ANSELAbits.ANSA0 = 1; // Ensure AN0/RA0 is analog

        /* Initialize and enable ADC module. Fcy = 1/60MHz */
        AD1CON1 = 0x0400; // ADON:OFF, ADSIDL:Cont, ADDMABM:DMA Scat/Gat, AD12B:12bit, FORM:UINT_RJ,
        // SSRC:MAN, SSRCG:0, ASAM:OnSAMP, SAMP:HOLD, DONE:status
        AD1CON2 = 0x0000; // VCFG:AVdd:AVss, CSCNA:OFF, CHPS:CH0, BUFS:status, SMPI:Int on done., BUFM:TOP, ALTS:MUXA
        AD1CON3 = 0x0008; // ADRC:SYSCLK, SAMC:Tad=0, ADCS:Tp*8
        AD1CON4 = 0x0000; // ADDMAEN:OFF, DMABL:1per
        AD1CHS0 = 0x0000; // Input Ch 0 Select: CH0NB:VrefL, CH0SB:AN0, CH0NA:VrefL, CH0SA:AN0
        AD1CHS123 = 0x0000; // CH123NB:VrefL, CH123SB:OA2/AN0, CH123NA:VrefL, CH123SA:OA2/AN0
        AD1CSSH = 0x0000; // CSS31:Skip, CSS30:Skip, CSS26:Skip, CSS25:Skip, CSS24:Skip
        AD1CSSL = 0x0000; // CSS:Skip
    } else {
        // Set up for Four Channels
        /* see also dsPIC33_FRM_ADC Section 16 Example 16-1 code. Example based on Fosc = 80MHz */
        /* dsOUC33xxx.pdf ref 11.2 mentions ANSELx register. Defined in s10.pdf dsPIC33_FRM_IO.pdf Sec 10.8.*/
        /* Set port configuration. Using PORTA and PORTB. i.e ANSELx = ANSELA */
        ANSELA = ANSELB = 0x0000;           // set all PORTA & PORTB bits to DIGITAL.
        ANSELAbits.ANSA0 = 1; // Ensure AN0/RA0 is analog
        ANSELAbits.ANSA1 = 1; // Ensure AN1/RA1 is analog
        ANSELBbits.ANSB0 = 1; // Ensure AN2/RB0 is analog
        ANSELBbits.ANSB1 = 1; // Ensure AN3/RB1 is analog

        /* Initialize and enable ADC module. Fcy = 1/60MHz */
#if 0
        // Manual sample and convert
        AD1CON1 = 0x0008; // ADON:OFF, ADSIDL:Cont, ADDMABM:DMA Scat/Gat, AD12B:10bit, FORM:UINT_RJ,
        // SSRC:MAN, SSRCG:0, SIMSAM:Simul, ASAM:OnSAMP, SAMP:HOLD, DONE:status
        AD1CON2 = 0x0300; // VCFG:AVdd:AVss, CSCNA:OFF, CHPS:CH0-3, BUFS:status, SMPI:Int on done., BUFM:TOP, ALTS:MUXA
        AD1CON3 = 0x0005; // ADRC:SYSCLK, SAMC:Tad=0, ADCS:Tp*5
        AD1CON4 = 0x0000; // ADDMAEN:OFF, DMABL:1per
        AD1CHS123 = 0x0000; // CH123NB:VrefL, CH123SB:OA2/AN0.AN1.AN2, CH123NA:VrefL, CH123SA:OA2/AN0.AN1.AN2
        AD1CHS0 = 0x0003; // Input Ch 0 Select: CH0NB:VrefL, CH0SB:na, CH0NA:VrefL, CH0SA:AN3
        AD1CSSH = 0x0000; // CSS31:Skip, CSS30:Skip, CSS26:Skip, CSS25:Skip, CSS24:Skip
        AD1CSSL = 0x0000; // CSS:Skip
#else
        // Auto sample and convert
        AD1CON1 = 0x00EC; // ADON:OFF, ADSIDL:Cont, ADDMABM:DMA Scat/Gat, AD12B:10bit, FORM:UINT_RJ,
        // SSRC:AUTO, SSRCG:0, SIMSAM:Simul, ASAM:OnConv, SAMP:HOLD, DONE:status
//        AD1CON2 = 0x0300; // VCFG:AVdd:AVss, CSCNA:OFF, CHPS:CH0-3, BUFS:status, SMPI:Int on done., BUFM:TOP, ALTS:MUXA
        AD1CON2 = 0x030C; // VCFG:AVdd:AVss, CSCNA:OFF, CHPS:CH0-3, BUFS:status, SMPI:N-1(3), BUFM:TOP, ALTS:MUXA
        AD1CON3 = 0x0305; // ADRC:SYSCLK, SAMC:Tad=3, ADCS:Tp*5
        AD1CON4 = 0x0000; // ADDMAEN:OFF, DMABL:1per
        AD1CHS123 = 0x0000; // CH123NB:VrefL, CH123SB:OA2/AN0.AN1.AN2, CH123NA:VrefL, CH123SA:OA2/AN0.AN1.AN2
        AD1CHS0 = 0x0003; // Input Ch 0 Select: CH0NB:VrefL, CH0SB:na, CH0NA:VrefL, CH0SA:AN3
        AD1CSSH = 0x0000; // CSS31:Skip, CSS30:Skip, CSS26:Skip, CSS25:Skip, CSS24:Skip
        AD1CSSL = 0x0000; // CSS:Skip
#endif

    }
    // Set up Interrupt
    IFS0bits.AD1IF = 0;     // reset interrupt flag if set.
    IPC3bits.AD1IP = 7;     // set interrupt priority to highest.
    IEC0bits.AD1IE = 1;     // enable ADC interrupts. 
    
    AD1CON1bits.ADON = 1;   // turn ON ADC.
    Delay_us(20);
}

void setLed(bool val) {
    if (val) {
        LATB |= (1 << 4); // set HIGH
    } else {
        LATB &= ~(1 << 4); // set LOW
    }
}
