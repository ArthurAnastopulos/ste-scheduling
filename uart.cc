#include "uart.h"

UART::UART_Registers_t * UART::regs;
UART::FIFO_Queue UART::RxFIFO;
UART::FIFO_Queue UART::TxFIFO;

extern "C"
{
   void __vector_18() __attribute__ ((signal));
   void __vector_19() __attribute__ ((signal));
}

void __vector_18()
{
   UART::udre_handler();
}


void __vector_19()
{
   UART::udre_handler();
}
