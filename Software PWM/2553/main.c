#include <msp430g2553.h>

#define LED     BIT0
#define BUTTON  BIT3

unsigned int count = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= LED;     // LED set as output
    P1OUT |= LED;     // Initially turns LED on
    P1REN |= BUTTON; //Enables resistor for button pin
    P1OUT |= BUTTON; //Enable resisotr as pullup
    P1IE |= BUTTON;   //enable interrupt for button
    P1IFG &= ~BUTTON;   //clear flag

    TA0CTL |=   TASSEL_2 + ID_1 + MC_3;     // SMCLK divide 2 UP/Down mode

    TA0CCR0 = 1000;              // Set clock period
    TA0CCTL0 |= CCIE;           // Enable Timer A capture/compare interrupt 0

    TA0CCR1 = 500;              //set duty cycle at 50%
    TA0CCTL1 |= CCIE;           //enable interrupt for timer

    TA1CTL |=   TASSEL_2 + ID_3 + MC_1; //SMCLK divide by 8 in up mode

    TA1CCR0 = 60000;            // Set clock period
    TA1CCTL0 |= CCIE;           // Enable Timer A capture/compare interrupt 1

    _BIS_SR(GIE + LPM0_bits); //low power mode with interrupts enabled
}


// Port 1 interrupt routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^= LED;   //flip led
    TA1CTL = TASSEL_2 | MC_1 | TACLR;   //SMCLK, Up mode, clear TA1R
    count+=1;       //increment counter
    switch(count)   //change duty cycle for each button press
    {
    case 1: TACCR1 = 600;break;
    case 2: TACCR1 = 700;break;
    case 3: TACCR1 = 800;break;
    case 4: TACCR1 = 900;break;
    case 5:
        {
            TACCR1 = 500;
            count = 0;
            break;
        }
    }
    P1IFG &= ~BUTTON; // Clear interrupt flag
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA1(void) {
    P1OUT ^= LED;   //flip LED
    TA0CCTL1 &= ~CCIFG;
}

