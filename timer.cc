#include "timer.h"


typedef unsigned long long Microseconds;

Timer0::TIMER0_Registers_t * Timer0::regs;
Timer0::timsk0_Register_t * Timer0::reg;
Microseconds Timer0::usec_counter;
LinkedList<Observer *, false, false, int> Timer0::observer;


extern "C"
{
    void __vector_16() __attribute__ ((signal));
}

void __vector_16()
{
   Timer0::timer_isr_handler();
}