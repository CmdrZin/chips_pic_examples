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

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISB = 0xFFEF;     // RB4 as output

    /* Setup analog functionality and port direction */
    //AN PortB as digital I/O
    ANSELB = 0xFFFF;

    /* Initialize peripherals */
}

void setLed(bool val) {
    if(val) {
        LATB |= (1<<4);        // set HIGH
    } else {
        LATB &= ~(1<<4);       // set LOW
    }
}
