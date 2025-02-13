#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

volatile uint8_t button_press = 0;
volatile uint8_t debounce;

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
    _delay_ms(20); // debounce delay
    // read status of PORT D 
    debounce = PIND;
    // Mask status of PD2
    debounce &= 0b00000100;

    // Verify PD2 status by comparing it to expected status according to ISR(last state)
    if(!button_press)
    {
        if(debounce == 0)
        {
            button_press = !button_press;
            if(button_press)
            {
                PORTB |= (1<<PB7); // set PIN7 to high 
            }
            else PORTB &= ~(1<<PB7);
        }
       
    }
    else
    {
        if(debounce == 0b00000100)
        {
            button_press = !button_press;
            if(button_press)
            {
                PORTB |= (1<<PB7); // set PIN7 to high 
            }
            else PORTB &= ~(1<<PB7);
        }
    }

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
