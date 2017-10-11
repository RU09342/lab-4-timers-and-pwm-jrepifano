#include <msp430.h> 

#include <msp430f5529.h>

#define LED BIT0;
#define BUTTON BIT1;

int count = 0;

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //kill wdt
    P1DIR |= LED; //set led to write
    P1SEL |= LED; //select led as the output

    P1REN |= BUTTON; //enable resistor for button
    P1OUT |= BUTTON; //set resistor to pullup
    P1IE |= BUTTON;   //enable interrupt for button
    P1IFG &= ~BUTTON;   //clear flag

    TB0CCR0 = 1000; //period set to 1000
    TB0CCTL1 = OUTMOD_7; //picks output mode for selected pin
    TB0CCR1 = 500; //period for CCR1 500/1000 is 50% duty cycle
    TB0CTL = TBSSEL_2 + MC_1; //SMCLK in up mode no divider

    __bis_SR_register(LPM0_bits); //low power mode
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
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
