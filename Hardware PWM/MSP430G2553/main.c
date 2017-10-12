/*
 * Mitchell Hay
 * RU09342
 * Lab 4 Hardware PWM
 * MSP430G2553
 */

#include <msp430.h>
volatile unsigned int i = 500;
volatile unsigned int btnPress = 0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT2;                       // P1.2 output
  P1SEL |= BIT2;                       // P1.2 options select
  
  // LED setup
  P1DIR |= BIT6;
  P1OUT &= ~BIT6;
	
  // Timer A0 setup
  TA0CCR0 = 1000-1;                          // PWM Period
  TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
  TA0CCR1 = i-1;                            // CCR1 PWM duty cycle
  TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR

  // Button setup
  P1DIR &= ~(BIT3); //set P1.1 input
  P1REN |= BIT3; //enable pull-up resistor
  P1OUT |= BIT3;
  P1IE |= BIT3; //enable the interrupt on P1.1
  P1IES |= ~BIT3; //set to look for falling edge
  P1IFG &= ~(BIT3); //clear interrupt flag

  // Timer A1 setup for debounce
  CCR0 = 4000;
  CCTL0 |= CCIE;
  TACTL |= TASSEL_2 + MC_1;

	__enable_interrupt();

  __bis_SR_register(GIE);             // Enable interrupts
  while(1){}
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_Debounce(void) {
	TA1CCTL0 &= ~CCIE; // Turn off interrupt enable
	TA1CTL = MC_0; // Stop timer
	P1IE |= BIT3; // Enable button interrupt
	if (btnPress == 1) {
		P1OUT ^= BIT6; // Toggle LED
		if (i >= 1000) {
			i = 1;
		}

		else {
			i += 100;
		}
	}
	TA0CCR1 = i - 1;
	btnPress = 0; // Clear button press
	P1IFG &= ~BIT3; // Clear flag
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void) {
	TA1CTL |= TASSEL_2 + MC_1; // Start Timer 1
	P1IE &= ~BIT3; // Turn off interrupt enable
	TA1CCTL0 |= CCIE; // interrupt enable for timer
	btnPress = 1; // Show button was pressed
	P1IFG &= ~BIT3; // Clear flag
}
