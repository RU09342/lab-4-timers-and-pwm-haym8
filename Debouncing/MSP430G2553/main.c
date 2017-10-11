#include <msp430.h>
unsigned int btnPress = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	P1DIR |= BIT0;
	P1OUT &= ~BIT0;

	P1DIR &= ~(BIT3); //set P1.1 input
	P1REN |= BIT3; //enable pull-up resistor
	P1OUT |= BIT3;
	P1IE |= BIT3; //enable the interrupt on P1.1
	P1IES |= ~BIT3; //set to look for falling edge
	P1IFG &= ~(BIT3); //clear interrupt flag

	CCR0 = 4000;
	CCTL0 |= CCIE;
	TACTL |= TASSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

	while (1) {
	}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Debounce(void) {
	TA0CCTL0 &= ~CCIE; // Turn off interrupt enable
	TA0CTL = MC_0; // Stop timer
	P1IE |= BIT3; // Enable button interrupt
	if (btnPress == 1) {
		P1OUT ^= BIT0; // Toggle LED
	}
	btnPress = 0;
	P1IFG &= ~BIT3; // Clear flag
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void) {
	TA0CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT3; // Turn off interrupt enable
	CCTL0 |= CCIE; //
	btnPress = 1;
	P1IFG &= ~BIT3; // Clear flag
}

