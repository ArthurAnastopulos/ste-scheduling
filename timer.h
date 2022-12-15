#ifndef __TIMER_H__
#define __TIMER_H__

#include "LinkedList.h"
#include "observer.h"

class Timer0
{
    public:
        typedef unsigned long long Microseconds;
        static const unsigned char TOP = 125;
        static const unsigned char MAX = 255;
        //const unsigned char prescale = 8; //Div

        Timer0()
        {
            regs = (TIMER0_Registers_t*) 0x44;
            reg = (timsk0_Register_t*) 0x6E;
            //zera contador
            usec_counter = 0;
            
            // Configurar prescaler

            //seta bits
            regs->tccr0a = 0x00;
            regs->tccr0b = 0x04; //CS01 -> bit 1
            reg->timsk0 = 0x01; //TOIE0 -> bit 0

            regs->tcnt0 = MAX - TOP + 1; //Timer Base
        }

        ~Timer0()
        {
            regs->tccr0a = 0;
            regs->tccr0b = 0;
            reg->timsk0 = 0;
        }

        static Microseconds micros() {return usec_counter;}

        static void timer_isr_handler()
        {
            regs->tcnt0 = MAX - TOP;
            usec_counter += 2000; //Timer Resolution
            Observer * obs = 0;
            for (int i = 0; i < observer.size(); i++)
            {
                obs = observer.get(i);
                obs->update(2000);
            }
            
        }

        void delay(Microseconds d)
        {
            Microseconds end = micros() + d;
            while ( end >= micros() );
        }

        void registerObserver(Observer * obs)
        {
            observer.insert(obs);
        }
    private:
        struct TIMER0_Registers_t
        {
            volatile unsigned char tccr0a;
            volatile unsigned char tccr0b;
            volatile unsigned char tcnt0;
            volatile unsigned char ocr0a;
            volatile unsigned char ocr0b;
        };
        static TIMER0_Registers_t * regs;

        struct timsk0_Register_t
        {
            volatile unsigned char timsk0;
        };
        static timsk0_Register_t * reg;

        static Microseconds usec_counter;

        static LinkedList<Observer *, false, false, int> observer;
};

#endif