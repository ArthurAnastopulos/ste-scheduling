// Arquivo: adc.h
// Autor: Arliones Hoeller, IFSC-SJE

#ifndef __ADC_H__
#define __ADC_H__

#include "cfifo.h"

class ADC
{
public:
    enum ADC_Channel_t
    {
        ADC_CH0 = 0,
        ADC_CH1,
        ADC_CH2,
        ADC_CH3,
        ADC_CH4,
        ADC_CH5,
        ADC_CH6,
        ADC_CH7,
        ADC_CH8,
        ADC_1V1 = 14,
        ADC_0V
    };

    ADC(ADC_Channel_t ch) : channel(ch)
    {
        set_channel();
        regs->adcsra =  (1 << 6) | (1 << 7) | (1 << 3) | (1 << 5) | 7; // Bit 3 – ADIE: ADC Interrupt // Enable Bit 5 – ADATE: ADC Auto Trigger Enable
        regs->adcsrb = 0;
    }

    int get()
    {
        return cfifo.average();
    }

    int get(int i)
    {
         return cfifo.get(i);
    }

    static void int_handler()
    {
        cfifo.enqueue(regs->adc);
    }
private:
    struct ADC_Registers_t
    {
        volatile unsigned int adc;
        volatile unsigned char adcsra;
        volatile unsigned char adcsrb;
        volatile unsigned char admux;
    };
    static ADC_Registers_t * regs;

    void set_channel()
    {
        regs->admux = (1 << 6) | (channel & 0x0f);
    }
    
    ADC_Channel_t channel;
    
    typedef CFIFO< 16, int> CircularFifo;   
    static CircularFifo cfifo;
};

#endif
