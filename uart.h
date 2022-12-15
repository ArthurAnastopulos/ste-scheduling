// Arquivo: uart.h
// Autor: Arliones Hoeller, IFSC-SJE
 
#ifndef __UART_H__
#define __UART_H__
 
#include "fifo.h"
 
class UART
{
private:
   struct UART_Registers_t
   {
       unsigned char ucsra;
       unsigned char ucsrb;
       unsigned char ucsrc;
       unsigned char reserved;
       unsigned char ubrrl;
       unsigned char ubrrh;
       unsigned char udr;
   };
   static UART_Registers_t * regs;
   
   typedef FIFO< unsigned char, 32> FIFO_Queue;
   static FIFO_Queue RxFIFO;
   static FIFO_Queue TxFIFO;
 
public:
   enum UART_Baudrate_t
   {
       UART_9600 = 103,
       UART_14400 = 68,
       UART_19200 = 51
   };
 
   UART(UART_Baudrate_t baud = UART_9600, int db = 8, int pr = 0, int sb = 1)
   {
       regs = (UART_Registers_t*) 0xc0;
       regs->ubrrh = (unsigned char) ((baud >> 8) & 0x0f);
       regs->ubrrl = (unsigned char)  (baud & 0x00ff);
       regs->ucsra = 0;
       regs->ucsrb = (1 << 4) | (1 << 3);
       regs->ucsrc = (3 << 1);
   }
 
   void put(char c)
   {
       __asm__ ("cli");
       TxFIFO.enqueue(c);
       __asm__ ("sei");
 
       // ligar interrupção UDRE
       regs->ucsrb |= (1 << 5); //(1 << UDRIE0)
   }
 
   void puts(char *s)
   {
       puts((const char *)s);
   }
   void puts(const char * s)
   {
       while(*s != '\0')
           put(*s++);
   }
 
   char get()
   {
       if(RxFIFO.size() > 0){
           unsigned char data;
           RxFIFO.dequeue(&data);
           return data;
       }
   }
 
   static void udre_handler()
   {
       unsigned char data;
       if (TxFIFO.size() > 0)
       {
           TxFIFO.dequeue(&data);
           regs->udr = data;
       }
       else
       {
           // Desligar interrupção UDRE
           regs->ucsrb &= ~(1 << 5);
       }
   }
 
   static void rxc_handler()
   {
       __asm__ ("cli");
       RxFIFO.enqueue(regs->udr);
       __asm__ ("sei"); 
   }
};
 
#endif
