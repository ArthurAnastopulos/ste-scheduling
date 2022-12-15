# Atividade 3 - Fila de funções e escalonamento

Neste repositorio esta desenvolvido uma  uma biblioteca para uso de componentes do <a href="https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf">ATmega328P</a> (MCU presente no Arduino Uno) utilizando o GCC. Onde o objetivo desta atividade é conhecer e aplicar um Alarme para o escalonamento agendado em um Sistema Multitarefas. A linguagem-base da biblioteca é C++, embora alguns trechos de código podem conter sequências C ou em assembly.

# Atividade

Esta atividade tem o intuito de realizar a implementação de duas aplicações (main) para o RMS (Rate Monotonic Scheduling) e EDD (Earliest Due Date) para replicar o comportamento dos escalonadores conforme as sequências de execução nos slides 14 e 15, para o RMS, e exemplos 1 e 2 do slide 17, para o EDD.

Para o primeiro exemplo implementado em RMS é mostrado uma escala que funciona, pois prioridade ocorre com sucesso no seu tempo de agendamento previsto.Já o segundo exemplo é mostrado uma escala que não funciona, pois a primeira tarefa possui um tempo de execução uma pequena taxa de erro, que leva a segunda tarefa a ter um delay no inicio de sua execução.

Para o primeiro exemplo implementado em EDD é mostrado um agendamento que funciona, pois tempo de execução das tarefas é menor que sua deadline. Já o segundo exemplo é mostrado um agendamento que não funciona, pois o tempo agendado para a terceira tarefa acaba não podendo ser atingido graças ao tempo de execeção das outras tarefas.

![](/diagrama-uml-alarme.png)

# Event

A estrutura Event, possui os atributos necessárias para o funcionamento do escalonamento na fila de funções, como por exemplo a prioridade de uma tarefa utilizada no RMS, para determinar a ordem das funções a serem executadas.

## Atributos
```cpp
    Timer0::Microseconds time;
    void (*func)(void*);
    void * args;
    Timer0::Microseconds release_time = Timer0::micros();
    char priority;
```
# Function Queue

A Fila de funções foi implamentada para ser utilizada como componente do Alarme, onde é gurdada a ordem da funções agendadas no sistema. Esta podendo ser uma `FIFO` ou uma `LinkedList`.

## Métodos

```cpp
    #if SCHED_TYPE == SCHED_FIFO
    class FunctionQueue : public FIFO<Event *, 16>
    void run()

    #if SCHED_TYPE == SCHED_RMS
    class FunctionQueue : public LinkedList<Event *, true, false, char>
    void run()

    #if SCHED_TYPE == SCHED_EDD
    class FunctionQueue : public LinkedList<Event *, true, false, Timer0::Microseconds>
    void run()
```

# Alarme

O Alarme implementado tem como objetivo agendar e atulizar a funções a serem executadas em um sistema.

## Atributos

```cpp
    LinkedList<Event *, true, true, Timer0::Microseconds> events;

    FunctionQueue * fqueue;
```

## Métodos

```cpp
    void update(int val)

    void schedule(Event * event_)
```

# Timer0

Nesta classe é implementado o Timer0 como visto no datasheet ATmega328P, que tem como função contar o tempo em microsegundos e gerar delays no sistema.

## Atributos

```cpp
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

        typedef unsigned long long Microseconds;

        static const unsigned char TOP = 125;

        static const unsigned char MAX = 255;
```

## Métodos

```cpp
    static Microseconds micros()

    static void timer_isr_handler()

    void delay(Microseconds d)

    void registerObserver(Observer * obs)
```

# LinkedList

Implementação da estrutura de dados: lista encadeada que pode ser ordenada e/ou relativa para um determinado tipo (char, Timer0::Microseconds, etc).

## Atributos

```cpp
    struct Element
    {
        Element *next, *prev;
        T data;
        T_ORDER rank;
    };

    Element *head, *tail;

    int _size;
```

## Métodos

```cpp
    void insert(T _data, T_ORDER _rank = 0)

    T get(int i)

    T remove_head()

    T_ORDER update_head_rank(T_ORDER upd)

    int size()

    void insert_ordered(T _data, T_ORDER _rank)

    void insert_tail(T _data, T_ORDER _rank)
```