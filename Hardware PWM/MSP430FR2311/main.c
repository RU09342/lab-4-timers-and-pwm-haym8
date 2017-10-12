/*
 * Mitchell Hay
 * RU09342
 * Lab 4 Hardware PWM
 * MSP430FR2311
 */

#include <msp430.h>
volatile unsigned int i = 500;
volatile unsigned int btnPress = 0;

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	// Disable the GPIO power-on default high-impedance mode
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= BIT6;                       // P1.6 output
	P1SEL1 |= BIT6;                       // P1.6 options select
	
	// LED setup
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	
	// Timer B0 setup
	TB0CCR0 = 1000 - 1;                          // PWM Period
	TB0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
	TB0CCR1 = i - 1;                            // CCR1 PWM duty cycle
	TB0CTL = TBSSEL_2 + MC_1 + TBCLR;         // SMCLK, up mode, clear TAR

	// Button setup
	P1DIR &= ~(BIT1); //set P1.1 input
	P1REN |= BIT1; //enable pull-up resistor
	P1OUT |= BIT1;
	P1IE |= BIT1; //enable the interrupt on P1.1
	P1IES |= ~BIT1; //set to look for falling edge
	P1IFG &= ~(BIT1); //clear interrupt flag

	// Timer A1 setup for debounce
	TB1CCR0 = 4000;
	TB1CCTL0 |= CCIE;
	TB1CTL |= TBSSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(GIE);             // Enter LPM0
	while (1) {
	}
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_Debounce(void) {
	TB1CCTL0 &= ~CCIE; // Turn off interrupt enable
	TB1CTL = MC_0; // Stop timer
	P1IE |= BIT1; // Enable button interrupt
	if (btnPress == 1) {
		P1OUT ^= BIT0; // Toggle LED
		if (i >= 1000) {
			i = 1;
		}

		else {
			i += 100;
		}
	}
	TB0CCR1 = i - 1;
	btnPress = 0; // Clear button press
	P1IFG &= ~BIT1; // Clear flag
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void) {
	TB1CTL |= TBSSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT1; // Turn off interrupt enable
	TB1CCTL0 |= CCIE; // interrupt enable for timer
	btnPress = 1; // Show button was pressed
	P1IFG &= ~BIT1; // Clear flag
}
