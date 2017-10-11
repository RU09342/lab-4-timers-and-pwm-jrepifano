#include <msp430fr2311.h>

#define LED     BIT0
#define BUTTON  BIT1

unsigned int count = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= LED;     // LED set as output
    P1OUT |= LED;     // Initially turns LED on
    P1REN |= BUTTON; //Enables resistor for button pin
    P1OUT |= BUTTON; //Enable resisotr as pullup
    P1IE |= BUTTON;   //enable interrupt for button
    P1IFG &= ~BUTTON;   //clear flag

    TB0CTL |=   TBSSEL_2 + ID_1 + MC_3;     // SMCLK divide 2 UP/Down mode

    TB0CCR0 = 1000;              // Set clock period
    TB0CCTL0 |= CCIE;           // Enable Timer A capture/compare interrupt 0

    TB0CCR1 = 500;              //set duty cycle at 50%
    TB0CCTL1 |= CCIE;           //enable interrupt for timer

    TB1CTL |=   TBSSEL_2 + ID_3 + MC_1; //SMCLK divide by 8 in up mode

    TB1CCR0 = 60000;            // Set clock period
    TB1CCTL0 |= CCIE;           // Enable Timer A capture/compare interrupt 1

    _BIS_SR(GIE + LPM0_bits); //low power mode with interrupts enabled
}


// Port 1 interrupt routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^= LED;   //flip led
    TB1CTL = TBSSEL_2 | MC_1 | TBCLR;   //SMCLK, Up mode, clear TB1R
    count+=1;       //increment counter
    switch(count)   //change duty cycle for each button press
    {
    case 1: TBCCR1 = 600;break;
    case 2: TBCCR1 = 700;break;
    case 3: TBCCR1 = 800;break;
    case 4: TBCCR1 = 900;break;
    case 5:
        {
            TBCCR1 = 500;
            count = 0;
            break;
        }
    }
    P1IFG &= ~BUTTON; // Clear interrupt flag
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0(void) {
    P1OUT ^= LED;   //flip LED
    TB0CCTL1 &= ~CCIFG;
}

