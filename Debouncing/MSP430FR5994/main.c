#include <msp430.h>
unsigned int btnPress = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	// Disable the GPIO power-on default high-impedance mode
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= BIT0;
	P1OUT &= ~BIT0;

	P5DIR &= ~(BIT6); //set P1.1 input
	P5REN |= BIT6; //enable pull-up resistor
	P5OUT |= BIT6;
	P5IE |= BIT6; //enable the interrupt on P1.1
	P5IES |= ~BIT6; //set to look for falling edge
	P5IFG &= ~(BIT6); //clear interrupt flag

	TA0CCR0 = 4000;
	TA0CCTL0 |= CCIE;
	TA0CTL |= TASSEL__SMCLK + MC_1;

	__enable_interrupt();

	__bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

	while (1) {
	}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Debounce(void) {
	TA0CCTL0 &= ~CCIE; // Turn off interrupt enable
	TA0CTL = MC_0; // Stop timer
	P5IE |= BIT6; // Enable button interrupt
	if (btnPress == 1) {
		P1OUT ^= BIT0; // Toggle LED
	}
	btnPress = 0;
	P5IFG &= ~BIT6; // Clear flag
}

#pragma vector = PORT5_VECTOR
__interrupt void PORT_5(void) {
	TA0CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P5IE &= ~BIT6; // Turn off interrupt enable
	TA0CCTL0 |= CCIE; //
	btnPress = 1;
	P5IFG &= ~BIT6; // Clear flag
}
