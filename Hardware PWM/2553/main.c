#include <msp430.h> 

#include <msp430g2553.h>

#define LED BIT0;
#define BUTTON BIT3;

int count = 0;

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; //kill wdt
    P1DIR |= LED; //set led to write
    P1SEL |= LED; //select led as the output

    P1REN |= BUTTON; //enable resistor for button
    P1OUT |= BUTTON; //set resistor to pullup
    P1IE |= BUTTON;   //enable interrupt for button
    P1IFG &= ~BUTTON;   //clear flag

    TA0CCR0 = 1000; //period set to 1000
    TA0CCTL1 = OUTMOD_7; //picks output mode for selected pin
    TA0CCR1 = 500; //period for CCR1 500/1000 is 50% duty cycle
    TA0CTL = TASSEL_2 + MC_1; //SMCLK in up mode no divider

    __bis_SR_register(LPM0_bits); //low power mode
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
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
