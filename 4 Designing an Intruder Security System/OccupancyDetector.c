/*
 * OccupancyDetector.c
 *
 *  Created on: Feb 1, 2023
 *      Author: Jillian To
 */

#include <msp430.h>

#define ARMED 0
#define WARNING 1
#define ALERT 2

char state = ARMED;
char detection = 0x00;

void gpioInit();

int main(void) {

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    gpioInit(); // initialize needed GPIO pins

    PM5CTL0 &= ~LOCKLPM5; // disable GPIO power-on default high-impedance mode to activate previously configured port settings

    __bis_SR_register(GIE); // enter LPM3 with interrupt

    while(1) {
        if(state == ARMED) {
            if(detection) {
                state = WARNING;
            } else {
                P6OUT ^= BIT6; // blink green LED
                __delay_cycles(3000000); // delay 3 seconds
            }

        }



        if(toggle && (state == ARMED)) {

        }

    }

}

void gpioInit() {

    // inputs
    P2DIR &= ~BIT3; // set P2.3 to input
    P4DIR &= ~BIT1; // set P4.1 to input
    P2REN |= BIT3; // enable P2.3 resistor
    P2OUT |= BIT3; // set P2.3 resistor to pull-up
    P4REN |= BIT1; // enable P4.1 resistor
    P4OUT |= BIT1; // set P4.1  resistor to pull-up
    P2IES &= ~BIT3; // P2.3 Low -> High edge
    P2IE |= BIT3; // P2.3 interrupt enabled
    P4IES &= ~BIT1; // P2.3 Low -> High edge
    P4IE |= BIT1; // P2.3 interrupt enabled

    // outputs
    P1OUT &= ~BIT0; // reset P1.0 latch
    P1DIR |= BIT0; // set P1.0 to output
    P6OUT &= ~BIT6; // reset P6.6 latch
    P6DIR |= BIT6; // set P6.6 to output

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
    P2IFG &= ~BIT3; // clear P2.3 IFG
    P2IES ^= BIT3; // switch edge type
    detection ^= 0x01; // enable detection if disabled, disable if enabled
}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void) {
    P4IFG &= ~BIT1; // clear P4.1 IFG
    state = ARMED; // reset system to armed state
}



