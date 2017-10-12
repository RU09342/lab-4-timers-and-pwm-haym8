/*
 * Mitchell Hay
 * RU09342
 * Lab 4 Debouncing Extra Work
 * MSP430F5529
 */

#include <msp430.h>
// Set up each button press
unsigned int btn1Press = 0;
unsigned int btn2Press = 0;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	// Set up LEDs
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	P4DIR |= BIT7;
	P4OUT &= ~BIT7;

	// Button 1 Setup
	P1DIR &= ~(BIT1); 	//set P1.1 input
	P1REN |= BIT1; 		//enable pull-up resistor
	P1OUT |= BIT1;		
	P1IE |= BIT1; 		//enable the interrupt on P1.1
	P1IES |= ~BIT1; 	//set to look for falling edge
	P1IFG &= ~(BIT1); 	//clear interrupt flag
	
	// Button 2 setup
	P2DIR &= ~(BIT1); 	//set P1.1 input
	P2REN |= BIT1; 		//enable pull-up resistor
	P2OUT |= BIT1;
	P2IE |= BIT1; 		//enable the interrupt on P1.1 (button
	P2IES |= ~BIT1; 	//set to look for falling edge
	P2IFG &= ~(BIT1); 	//clear interrupt flag

	// Set up Timer A to handle debouncing
	TA0CCR0 = 4000;
	TA0CCTL0 |= CCIE;
	TA0CTL |= TASSEL_2 + MC_1;

	__enable_interrupt();

	__bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

	// Loop forever
	while (1) {
	}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Debounce(void) {
	TA0CCTL0 &= ~CCIE; 	// Turn off interrupt enable for Timer A
	TA0CTL = MC_0; 		// Stop timer
	P1IE |= BIT1; 		// Enable button interrupt
	P2IE |= BIT1; 		// Enable button interrupt
	if (btn1Press == 1) {
		P4OUT ^= BIT7; 	// Toggle LED
	}
	if (btn2Press == 1) {
		P1OUT ^= BIT0; 	// Toggle LED
	}
	btn1Press = 0;		// Reset button 1 press
	btn2Press = 0;		// Reset button 2 press
	P1IFG &= ~BIT1; 	// Clear flag
	P2IFG &= ~BIT1; 	// Clear flag
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void) {
	TA0CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT1; 		// Turn off interrupt enable for Button 1
	TA0CCTL0 |= CCIE; 	// Turn on interrupt enable for Timer A
	btn1Press = 1;		// Show button was pressed
	P1IFG &= ~BIT1; 	// Clear flag
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT_2(void) {
	TA0CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P2IE &= ~BIT1; 		// Turn off interrupt enable for Button 2
	TA0CCTL0 |= CCIE; 	// Turn on interrupt enable for Timer A
	btn2Press = 1;		// Show button was pressed
	P2IFG &= ~BIT1; 	// Clear flag
}
