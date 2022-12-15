#ifndef __FUNCTIONQUEUE_H__
#define __FUNCTIONQUEUE_H__

#include "fifo.h"
#include "timer.h"
#include "LinkedList.h"
#include "defs.h"

struct Event
{
    Timer0::Microseconds time; //EDD, considerar que dealine é igual ao perido/time
    void (*func)(void*);
    void * args;
    Timer0::Microseconds release_time = Timer0::micros();
    char priority; //int 8 bits
};

#if SCHED_TYPE == SCHED_FIFO
class FunctionQueue : public FIFO<Event *, 16>
{
    private:

    public:

        void run()
        {
            Event * next;
            //cli
            __asm__ ("cli");
            while ( this->size() > 0)
            {
                this->dequeue(&next);
                //sei
                __asm__ ("sei");
                next->func(next->args);
                //cli
                __asm__ ("cli");
            }
            //sei
            __asm__ ("sei");
        }
};
#elif SCHED_TYPE == SCHED_RMS
class FunctionQueue : public LinkedList<Event *, true, false, char> // char da prioridade trocar para o tempo
{
    private:

    public:

    void run()
    {
        Event  * next;
        __asm__ ("cli");
        while ( this->size() > 0)
        {
            next = this->remove_head();
            __asm__("sei");
            next->func(next->args);
            __asm__("cli");
        }
        __asm__("sei");
    }

};
#elif SCHED_TYPE == SCHED_EDD
class FunctionQueue : public LinkedList<Event *, true, false, Timer0::Microseconds>
{
    private:

    public:

    void run()
    {
        Event  * next;
        __asm__ ("cli");
        while ( this->size() > 0)
        {
            next = this->remove_head();
            __asm__("sei");
            next->func(next->args);
            __asm__("cli");
        }
        __asm__("sei");
    }

};
#endif

#endif