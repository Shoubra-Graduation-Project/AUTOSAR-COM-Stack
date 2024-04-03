// Timer3.h
// Runs on LM4F120/TM4C123
// Use Timer3 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// March 20, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
  Program 7.5, example 7.6

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#ifndef __TIMER_H__ // do not include more than once
#define __TIMER_H__


#define TIMERS_TIMER0A 0
#define TIMERS_TIMER1A 1
#define TIMERS_TIMER2A 2
#define TIMERS_TIMER3A 3
#define TIMERS_TIMER4A 4
#define TIMERS_TIMER5A 5
#define TIMERS_TIMER0B 6
#define TIMERS_TIMER1B 7
#define TIMERS_TIMER2B 8
#define TIMERS_TIMER3B 9
#define TIMERS_TIMER4B 10
#define TIMERS_TIMER5B 11

void Timer0A_Init(void(*task)(void), unsigned long period);
void Timer1A_Init(void(*task)(void), unsigned long period);
void Timer2A_Init(void(*task)(void), unsigned long period);
void Timer3A_Init(void(*task)(void), unsigned long period);
void Timer4A_Init(void(*task)(void), unsigned long period);
void Timer5A_Init(void(*task)(void), unsigned long period);
void Timer0B_Init(void(*task)(void), unsigned long period);
void Timer1B_Init(void(*task)(void), unsigned long period);
void Timer2B_Init(void(*task)(void), unsigned long period);
void Timer3B_Init(void(*task)(void), unsigned long period);
void Timer4B_Init(void(*task)(void), unsigned long period);
void Timer5B_Init(void(*task)(void), unsigned long period);

void Timer0A_LoadValue(unsigned long period);
void Timer1A_LoadValue(unsigned long period);
void Timer2A_LoadValue(unsigned long period);
void Timer3A_LoadValue(unsigned long period);
void Timer4A_LoadValue(unsigned long period);
void Timer5A_LoadValue(unsigned long period);
void Timer0B_LoadValue(unsigned long period);
void Timer1B_LoadValue(unsigned long period);
void Timer2B_LoadValue(unsigned long period);
void Timer3B_LoadValue(unsigned long period);
void Timer4B_LoadValue(unsigned long period);
void Timer5B_LoadValue(unsigned long period);

void Timer0_Enable();
void Timer1_Enable();
void Timer2_Enable();
void Timer3_Enable();
void Timer4_Enable();
void Timer5_Enable();
void Timer0_Disable();
void Timer1_Disable();
void Timer2_Disable();
void Timer3_Disable();
void Timer4_Disable();
void Timer5_Disable();

#endif // __TIMER2INTS_H__
