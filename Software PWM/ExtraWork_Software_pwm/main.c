/*
 * Mitchell Hay
 * Lab 4 - Software PWM
 * MSP430F5529
 */
#include <msp430.h>

// Set initial duty cycle to 50%
volatile unsigned int i = 500;
// Initialize variable to hold button press
volatile unsigned int btnPress = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	// LED Setup
	P1DIR |= BIT0;
	P4DIR |= BIT7;
	P4OUT &= ~BIT7;

	// Button Setup
	P1DIR &= ~(BIT1); //set P1.1 input
	P1REN |= BIT1; //enable pull-up resistor
	P1OUT |= BIT1;
	P1IE |= BIT1; //enable the interrupt on P1.1
	P1IES |= ~BIT1; //set to look for falling edge
	P1IFG &= ~(BIT1); //clear interrupt flag

	// Timer A0 setup
	TA0CCTL1 = CCIE; 			// Interrupt enabled for CCR1
	TA0CCTL0 = CCIE;			// Interrupt enabled for CCR0
	TA0CTL = TASSEL_2 + MC_1; 		// SMCLK, up mode
	TA0CCR0 = 1000 - 1;			// 1 kHz frequency
	TA0CCR1 = i - 1;			// 50% duty cycle to start

	// Timer A1 setup for debounce handling
	TA1CCR0 = 4000;
	TA1CCTL0 |= CCIE;
	TA1CTL |= TASSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

	while (1) {
	}
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {
	if (i != 1)
		P1OUT |= BIT0; // LED on when CCR0 hit
	TA0CCTL0 &= ~BIT0; // Clear flag
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void) {
	if (TA0CCR1 != (1000 - 1))
		P1OUT &= ~BIT0; //LED off when CCR1 hit
	TA0CCTL1 &= ~BIT0; // Clear flag
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_Debounce(void) {
	TA1CCTL0 &= ~CCIE; // Turn off interrupt enable
	TA1CTL = MC_0; // Stop timer
	P1IE |= BIT1; // Enable button interrupt
	if (btnPress == 1) {
		P4OUT ^= BIT7; // Toggle LED
		if (i >= 1000) {
			// Reset CCR1 when i goes above or equal to CCR0
			i = 1;
		} else {
			// Increase by factor of SQRT(2)
			// Linearizes brightness
			switch (i) {
			case 1:
				i = 2;
				break;
			case 2:
				i = 3;
				break;
			case 3:
				i = 4;
				break;
			case 4:
				i = 6;
				break;
			case 6:
				i = 8;
				break;
			case 8:
				i = 11;
				break;
			case 11:
				i = 16;
				break;
			case 16:
				i = 22;
				break;
			case 22:
				i = 31;
				break;
			case 31:
				i = 44;
				break;
			case 44:
				i = 63;
				break;
			case 63:
				i = 88;
				break;
			case 88:
				i = 125;
				break;
			case 125:
				i = 177;
				break;
			case 177:
				i = 250;
				break;
			case 250:
				i = 354;
				break;
			case 354:
				i = 500;
				break;
			case 500:
				i = 707;
				break;
			case 707:
				i = 1000;
				break;
			}
		}
	}
	TA0CCR1 = i - 1;
	btnPress = 0;	// Clear button press
	P1IFG &= ~BIT1; // Clear flag
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void) {
	TA1CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT1; // Turn off interrupt enable
	TA1CCTL0 |= CCIE; // Enable timer interrupt
	btnPress = 1; // Show button was pressed
	P1IFG &= ~BIT1; // Clear flag
}
