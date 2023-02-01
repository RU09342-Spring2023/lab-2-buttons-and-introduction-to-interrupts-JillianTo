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

char counter = 0;
char state = ARMED;
char detection = 0x01;

void gpioInit();

int main(void) {

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    gpioInit(); // initialize needed GPIO pins

    PM5CTL0 &= ~LOCKLPM5; // disable GPIO power-on default high-impedance mode to activate previously configured port settings

    __bis_SR_register(GIE); // enter LPM3 with interrupt

    while(1) {
        switch(state) {

        case ARMED:
            if(detection) { // when person is detected
                P6OUT &= ~BIT6; // reset green LED
                state = WARNING; // change state to warning
            } else { // when no detection
                P6OUT ^= BIT6; // blink green LED
                __delay_cycles(3000000); // delay 3 seconds
            }
        case WARNING:
            if((counter > 20) && detection) { // if 10 seconds has passed and person still detected
                state = ALERT; // change state to alert
                counter = 0; // reset counter
                P1OUT |= BIT0; // switch red LED on
            } else if(detection) { // if person still detected but 10 seconds has not passed
                P1OUT ^= BIT0; // blink red LED
                __delay_cycles(500000); // delay 500ms
                counter++;
            } else { // person not detected
                counter = 0; // reset counter
                P1OUT &= ~BIT0; // reset red LED
                state = ARMED; // change state to armed
            }

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
    P1OUT &= ~BIT0; // reset red LED
}



