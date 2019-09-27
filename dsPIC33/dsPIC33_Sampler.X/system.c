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

#include "system.h"          /* variables/params used by system.c             */

#define FP 60000000         // 60 MHz clk
//#define BAUDRATE 9600       // up to 115200
#define BAUDRATE 115200
#define BRGVAL ((FP/BAUDRATE)/16)-1

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration functions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c.                                          */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching using the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.*/

/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void) {
    /* Disable Watch Dog Timer */
    RCONbits.SWDTEN = 0;

#if 1
    // 60MHz..Configure PLL pre-scaler, PLL post-scaler, PLL divisor.
    // For 70 MHz, use M=76
    PLLFBD = 63; // M=65
    CLKDIVbits.PLLPRE = 0;      // N1=3
    CLKDIVbits.PLLPOST = 0;     // N2=2

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);
#else
    // 40MHz..Configure PLL pre-scaler, PLL post-scaler, PLL divisor.
    PLLFBD = 63;                // M = 65 for 7.37MHz internal RC
    CLKDIVbits.PLLPRE = 1;      // N1 = 3
    CLKDIVbits.PLLPOST = 0;     // N2 = 2
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

/*
 * Configure UART (CPUclk = 60 MHz)
 * Use PPS system to map UART to I/O pins
 */
void ConfigureUART(void)
{
    //*************************************************************
    // Unlock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

//    RPINR18bits.U1RXR = 0b00101011;     // map U1RXR to RB11.RP43
    RPINR18 = 0b00101011;     // map U1RXR to RB11.RP43
//    RPOR4bits.RP42R = 0b000001;          // map U1TX to RB10/RP42
    RPOR4 = 0b000001;          // map U1TX to RB10/RP42

    //*************************************************************
    // Lock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON | (1<<6));

    // UART set up.
    U1MODEbits.STSEL = 0;   // 1-stop bit
    U1MODEbits.PDSEL = 0;   // No Parity, 8-data bits
    U1MODEbits.ABAUD = 0;   // Auto-Baud disabled
    U1MODEbits.BRGH = 0;    // Standard-Speed mode. 1=high precision???
    
    U1BRG = BRGVAL;         // Baud Rate setting from calc macro 9600    
    
    U1STAbits.URXISEL = 0;  // Interrupt after one RX char received.

    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX char is transmitted.
    U1STAbits.UTXISEL1 = 0;

    IEC0bits.U1TXIE = 1;    // Enable UART TX interrupt
    
    U1STA = 0;              // clear status
    U1MODEbits.UARTEN = 1;  // Enable UART RX
    U1STAbits.UTXEN = 1;    // Enable UART TX
    
    // Check U1STAbits.URXDA == 1 for data available. U1RXREG has data.
    // If U1STAbits.OERR == 1, set to 0 to continue.
    // Write to U1TXREG to send data.
    // TX and RX buffers are 4 level FIFOs.
}
/*
 * Configure PWM Ch1 (CPUclk = 60 MHz)
 */
void ConfigurePWM(void) {
    //*************************************************************
    // Unlock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));

    IOCON1 = 0xCC00;        // PWM Mode to Independent
    FCLCON1 = 0x0003;       // Configure Faults

    //*************************************************************
    // Lock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON | (1<<6));

    PHASE1 = 2048; // period  600000 = 2kHz, 12000 = 10kHz.
    PDC1 = 1024; // 50% duty cycle
    DTR1 = 0; // Dead time
    ALTDTR1 = 0;
    PWMCON1 = 0x0200; // Ind time base, edge-align, independent duty cycle
    PTCON2 = 0x0000; // 1:1 pre-scaler

    // Set up trigger compare for interrupt on period.
  	TRIG1 = 0x0010;
	PWMCON1bits.TRGIEN = 1;
    TRGCON1 = 0x0000;                   // no delay
    IFS5bits.PWM1IF = 0;        // enable by resetting interrupt flag
    IEC5bits.PWM1IE = 1;
    IPC23bits.PWM1IP = 0b001; //interrupt priority level 0 .
  	PTCONbits.SEIEN = 1;
    
    
    PTCON = 0x8000; // Enable PWM Mode
}

/*
 * N us delay loop.
 */
void Delay_us(unsigned int delay){
    unsigned int i;

    for (i = 0; i < delay; i++){
        __asm__ volatile ("repeat #59");
        __asm__ volatile ("nop");
    }
}
