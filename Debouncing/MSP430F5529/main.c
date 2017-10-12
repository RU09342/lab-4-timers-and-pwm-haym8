/*
 *
 */
#include <msp430.h>
unsigned int btnPress = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	// Set up LEDs
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	P4DIR |= BIT7;
	P4OUT &= ~BIT7;

	// Set up button
	P1DIR &= ~(BIT1); //set P1.1 input
	P1REN |= BIT1; //enable pull-up resistor
	P1OUT |= BIT1;
	P1IE |= BIT1; //enable the interrupt on P1.1
	P1IES |= ~BIT1; //set to look for falling edge
	P1IFG &= ~(BIT1); //clear interrupt flag

	// Set up Timer A for debounce handling
	TA0CCR0 = 4000;
	TA0CCTL0 |= CCIE;
	TA0CTL |= TASSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

	// Loop forever
	while (1) {}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Debounce(void) {
	TA0CCTL0 &= ~CCIE; // Turn off interrupt enable
	TA0CTL = MC_0; // Stop timer
	P1IE |= BIT1; // Enable button interrupt
	if (btnPress == 1) {
		P4OUT ^= BIT7; // Toggle LED
	}
	btnPress = 0;	// Clear button press
	P1IFG &= ~BIT1; // Clear flag
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void) {
	TA0CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT1; // Turn off interrupt enable button
	TA0CCTL0 |= CCIE; // Turn on interrupt enable timer
	btnPress = 1;	// Show button was pressed
	P1IFG &= ~BIT1; // Clear flag
}
