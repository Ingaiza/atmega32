#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 800000000UL
#endif

volatile uint8_t button_press = 0;

void interrupt_init()
{
    // Setup PORTD INT0(PIN D2) as an Input
    DDRD &= ~(1<<PD2);
    // Activating Internal Pullup resistor for a normaly high logic
    PORTD |= (1<<PD2);
    // Enable INT0 Interrupt Service in the GICR register
    GICR |= (1<<INT0);
    // Set up INT0 as edge triggered in the MCUCR Register
    MCUCR = 0x01;
    // enable interrupts
    sei();

}


ISR(INT0_vect)
{
    button_press = !button_press;

    // Setup Interrupt Service Routine for INT0
    if(button_press)
    {
        PORTB |= (1<<PB7); // toggles PIN7 and latches the state 
    }
    else PORTB &= ~(1<<PB7);

}

int main()
{
    //Initializing Port B Pin 7 as an Output PIN
    DDRB |= (1<<PB7);
    //Setup interrupt
    interrupt_init();

    while(1)
    {
        if(!button_press)
        {
            PORTB ^= (1<<PB7);
            _delay_ms(1000);
        }
    }
}
