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
#include "system.h"         // access Dleay_us()

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(USER_ADC_CONF config) {
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISB = 0xFFEF; // RB4 as output

    /* Setup analog functionality and port direction */
    //AN PortB as digital I/O
    ANSELB = 0x0000;

    /* Initialize peripherals */

    if (config == USER_ONE_CHAN) {
        /* dsPIC33_FRM_ADC Section 16 Example 16-1 code. */
        // Example based on Fosc = 80MHz
        /* Set port configuration */
        ANSELA = ANSELB = 0x0000;
        ANSELAbits.ANSA0 = 1; // Ensure AN0/RA0 is analog

        /* Initialize and enable ADC module */
        AD1CON1 = 0x0400; // ADON:OFF, ADSIDL:Cont, ADDMABM:DMA Scat/Gat, AD12B:12bit, FORM:UINT_RJ,
        // SSRC:MAN, SSRCG:0, ASAM:OnSAMP, SAMP:HOLD, DONE:status
        AD1CON2 = 0x0000; // VCFG:AVdd:AVss, CSCNA:OFF, CHPS:CH0, BUFS:status, SMPI:Int on done., BUFM:TOP, ALTS:MUXA
        AD1CON3 = 0x000F; // ADRC:SYSCLK, SAMC:Tad=0, ADCS:Tp*15
        AD1CON4 = 0x0000; // ADDMAEN:OFF, DMABL:1per
        AD1CHS0 = 0x0000; // Input Ch 0 Select: CH0NB:VrefL, CH0SB:AN0, CH0NA:VrefL, CH0SA:AN0
        AD1CHS123 = 0x0000; // CH123NB:VrefL, CH123SB:OA2/AN0, CH123NA:VrefL, CH123SA:OA2/AN0
        AD1CSSH = 0x0000; // CSS31:Skip, CSS30:Skip, CSS26:Skip, CSS25:Skip, CSS24:Skip
        AD1CSSL = 0x0000; // CSS:Skip
    } else {
        // Set up for Four Channels
        /* dsPIC33_FRM_ADC Section 16 Example 16-1 code. */
        // Example based on Fosc = 80MHz
        /* Set port configuration */
        ANSELA = ANSELB = 0x0000;
        ANSELAbits.ANSA0 = 1; // Ensure AN0/RA0 is analog
        ANSELAbits.ANSA1 = 1; // Ensure AN1/RA1 is analog
        ANSELBbits.ANSB0 = 1; // Ensure AN2/RB0 is analog
        ANSELBbits.ANSB1 = 1; // Ensure AN3/RB1 is analog

        /* Initialize and enable ADC module */
        AD1CON1 = 0x0008; // ADON:OFF, ADSIDL:Cont, ADDMABM:DMA Scat/Gat, AD12B:10bit, FORM:UINT_RJ,
        // SSRC:MAN, SSRCG:0, SIMSAM:Simul, ASAM:OnSAMP, SAMP:HOLD, DONE:status
        AD1CON2 = 0x0300; // VCFG:AVdd:AVss, CSCNA:OFF, CHPS:CH0-3, BUFS:status, SMPI:Int on done., BUFM:TOP, ALTS:MUXA
        AD1CON3 = 0x0005; // ADRC:SYSCLK, SAMC:Tad=0, ADCS:Tp*5
//        AD1CON3 = 0x8000; // ADRC:IntRC, SAMC:n\a, ADCS:n\a
        AD1CON4 = 0x0000; // ADDMAEN:OFF, DMABL:1per
        AD1CHS123 = 0x0000; // CH123NB:VrefL, CH123SB:OA2/AN0.AN1.AN2, CH123NA:VrefL, CH123SA:OA2/AN0.AN1.AN2
        AD1CHS0 = 0x0003; // Input Ch 0 Select: CH0NB:VrefL, CH0SB:na, CH0NA:VrefL, CH0SA:AN3
        AD1CSSH = 0x0000; // CSS31:Skip, CSS30:Skip, CSS26:Skip, CSS25:Skip, CSS24:Skip
        AD1CSSL = 0x0000; // CSS:Skip
    }
    AD1CON1bits.ADON = 1;
    Delay_us(20);
}

void setLed(bool val) {
    if (val) {
        LATB |= (1 << 4); // set HIGH
    } else {
        LATB &= ~(1 << 4); // set LOW
    }
}
