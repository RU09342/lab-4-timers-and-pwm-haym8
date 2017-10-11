/*
 *
 */
#include <msp430.h>
unsigned int btn1Press = 0;
unsigned int btn2Press = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	P4DIR |= BIT7;
	P4OUT &= ~BIT7;

	P1DIR &= ~(BIT1); //set P1.1 input
	P1REN |= BIT1; //enable pull-up resistor
	P1OUT |= BIT1;
	P1IE |= BIT1; //enable the interrupt on P1.1
	P1IES |= ~BIT1; //set to look for falling edge
	P1IFG &= ~(BIT1); //clear interrupt flag

	P2DIR &= ~(BIT1); //set P1.1 input
	P2REN |= BIT1; //enable pull-up resistor
	P2OUT |= BIT1;
	P2IE |= BIT1; //enable the interrupt on P1.1
	P2IES |= ~BIT1; //set to look for falling edge
	P2IFG &= ~(BIT1); //clear interrupt flag

	TA0CCR0 = 4000;
	TA0CCTL0 |= CCIE;
	TA0CTL |= TASSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

	while (1) {
	}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Debounce(void) {
	TA0CCTL0 &= ~CCIE; // Turn off interrupt enable
	TA0CTL = MC_0; // Stop timer
	P1IE |= BIT1; // Enable button interrupt
	P2IE |= BIT1; // Enable button interrupt
	if (btn1Press == 1) {
		P4OUT ^= BIT7; // Toggle LED
	}
	if (btn2Press == 1) {
		P1OUT ^= BIT0; // Toggle LED
	}
	btn1Press = 0;
	btn2Press = 0;
	P1IFG &= ~BIT1; // Clear flag
	P2IFG &= ~BIT1; // Clear flag
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void) {
	TA0CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT1; // Turn off interrupt enable
	TA0CCTL0 |= CCIE; //
	btn1Press = 1;
	P1IFG &= ~BIT1; // Clear flag
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT_2(void) {
	TA0CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P2IE &= ~BIT1; // Turn off interrupt enable
	TA0CCTL0 |= CCIE; //
	btn2Press = 1;
	P2IFG &= ~BIT1; // Clear flag
}
