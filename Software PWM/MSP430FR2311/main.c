#include <msp430.h>

volatile unsigned int i = 500;
volatile unsigned int btnPress = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	// Disable the GPIO power-on default high-impedance mode
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	P2DIR |= BIT0;
	P2OUT &= ~BIT0;

	P1DIR &= ~(BIT1); //set P1.1 input
	P1REN |= BIT1; //enable pull-up resistor
	P1OUT |= BIT1;
	P1IE |= BIT1; //enable the interrupt on P1.1
	P1IES |= ~BIT1; //set to look for falling edge
	P1IFG &= ~(BIT1); //clear interrupt flag

	TB0CCTL1 = CCIE; 			// Interrupt enabled for CCR1
	TB0CCTL0 = CCIE;			// Interrupt enabled for CCR0
	TB0CTL = TBSSEL_2 + MC_1; 	// SMCLK, up mode
	TB0CCR0 = 1000 - 1;			// 1 kHz frequency
	TB0CCR1 = i - 1;			// 50% duty cycle to start

	TB1CCR0 = 4000;
	TB1CCTL0 |= CCIE;
	TB1CTL |= TBSSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(GIE); // enable interrupts in LPM0

	while (1) {
	}
}
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0(void) {
	if (i != 1)
		P1OUT |= BIT0; // LED on
	TB0CCTL0 &= ~BIT0;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1(void) {
	if (TB0CCR1 != 1000 - 1)
		P1OUT &= ~BIT0; //LED off
	TB0CCTL1 &= ~BIT0;
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_Debounce(void) {
	TB1CCTL0 &= ~CCIE; // Turn off interrupt enable
	TB1CTL = MC_0; // Stop timer
	P1IE |= BIT1; // Enable button interrupt
	if (btnPress == 1) {
		P2OUT ^= BIT0; // Toggle LED
		if (i >= 1000) {
			i = 1;
		}

		else {
			i += 100;
		}
	}
	TB0CCR1 = i - 1;
	btnPress = 0;
	P1IFG &= ~BIT1; // Clear flag
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void) {
	TB1CTL |= TBSSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT1; // Turn off interrupt enable
	TB1CCTL0 |= CCIE; //
	btnPress = 1;
	P1IFG &= ~BIT1; // Clear flag
}
