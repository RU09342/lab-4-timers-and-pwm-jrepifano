# Software Debouncing
For this part of the lab I just built off of my button interrupt code. 
I tested the button bouncing on the scope, the screenshots are in this folder.
It only takes a couple microseconds for this button to settle.
I used a continous interrupt which counts to 65000 with a clock frequency of 1MHz.
This means the interrupt will go off every 65ms, which is plenty of time for the button to settle.
When the button interrupt fire, it disables button interrupts.
When the timer interrupt fires, it enables button interrupts.
This will give a minimum 65ms delay in between each button press ensuring that the button settles.


The code for the 2553 is included below:


#include <msp430.h> 

#define BUTTON BIT3
#define LED BIT0


int blahh = 0; //variable to keep track of button interrupts

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer


P1DIR |= LED; // Set LEDS as outputs
P1OUT &= ~LED; // Initialize the LEDs off
P1REN |= BUTTON; //Enables a resistor for the button.
P1OUT |= BUTTON; //Setting the resistor as a pull-up.
P1IES |= BUTTON; //Sets Edge so that the button is activated when pressed down.

P1IE |= BUTTON; //Sets mask so that interrupt can't be ignored

CCTL0 = CCIE;                            // CCR0 interrupt enabled
TACTL = TASSEL_2 + MC_2;                 // SMCLK set to continuous mode

__enable_interrupt(); // enables interrupt for board

while (1) //infinite loooooooooooop
{

if(blahh > 0)
{
P1OUT ^= LED; // flipping everytime it loops

__delay_cycles(100000); // slow down clock so you can actually see the blinking
}
}

}

// Interrupt routine for port 1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
blahh ^= 0x01;  // flips LED so that the if statement to blink the LED is activated.
P1IE &= ~BUTTON; //disable button interrupts
P1IFG &= ~BUTTON; // Clear flag when done
P1OUT &= ~LED; // Turn's LEDs off after flag is cleared.

}

#pragma vector=TIMER0_A0_VECTOR               //Interrupt routine

__interrupt void Timer_A (void)
{

  P1IE |= BUTTON;                       //enable button interrupts

}

