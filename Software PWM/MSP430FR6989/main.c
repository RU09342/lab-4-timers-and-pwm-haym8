/*
 * Mitchell Hay
 * RU09342
 * Lab 4 Software PWM
 * MSP430FR6989
 */

#include <msp430.h>

volatile unsigned int i = 500;
volatile unsigned int btnPress = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	// Disable the GPIO power-on default high-impedance mode
	PM5CTL0 &= ~LOCKLPM5;

	// LED setup
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	P9DIR |= BIT7;
	P9OUT &= ~BIT7;

	// Button setup
	P1DIR &= ~(BIT1); //set P1.1 input
	P1REN |= BIT1; //enable pull-up resistor
	P1OUT |= BIT1;
	P1IE |= BIT1; //enable the interrupt on P1.1
	P1IES |= ~BIT1; //set to look for falling edge
	P1IFG &= ~(BIT1); //clear interrupt flag

	// Timer A0 setup
	TA0CCTL1 = CCIE; 			// Interrupt enabled for CCR1
	TA0CCTL0 = CCIE;			// Interrupt enabled for CCR0
	TA0CTL = TASSEL_2 + MC_1; 	// SMCLK, up mode
	TA0CCR0 = 1000 - 1;			// 1 kHz frequency
	TA0CCR1 = i - 1;			// 50% duty cycle to start

	// Timer A1 setup for debounce
	TA1CCR0 = 4000;
	TA1CCTL0 |= CCIE;
	TA1CTL |= TASSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(GIE); // enable interrupts in LPM0

	while (1) {
	}
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {
	if (i != 1)
		P1OUT |= BIT0; // LED on
	TA0CCTL0 &= ~BIT0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void) {
	if (TA0CCR1 != 1000 - 1)
		P1OUT &= ~BIT0; //LED off
	TA0CCTL1 &= ~BIT0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_Debounce(void) {
	TA1CCTL0 &= ~CCIE; // Turn off interrupt enable
	TA1CTL = MC_0; // Stop timer
	P1IE |= BIT1; // Enable button interrupt
	if (btnPress == 1) {
		P9OUT ^= BIT7; // Toggle LED
		if (i >= 1000) {
			i = 1;
		}

		else {
			i += 100;
		}
	}
	TA0CCR1 = i - 1;
	btnPress = 0; // Clear button press
	P1IFG &= ~BIT1; // Clear flag
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void) {
	TA1CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT1; // Turn off interrupt enable
	TA1CCTL0 |= CCIE; // enable interrupt for timer
	btnPress = 1; // Show button was pressed
	P1IFG &= ~BIT1; // Clear flag
}
