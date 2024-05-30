
#include "../include/tm4c123gh6pm.h"
#include "../include/tm4c123gh6pm11.h"

void (*PeriodicTask0A)(void);   // user function

// ***************** Timer0_Init ****************
// Activate TIMER0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer0_Enable()
{
	  TIMER0_CTL_R |= 0x00000001;    
}
void Timer0_Disable()
{
	  TIMER0_CTL_R &= ~(0x00000001);    
}

void Timer0A_Init(void(*task)(void), unsigned long period){
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  PeriodicTask0A = task;          // user function
  TIMER0_CTL_R &= ~(0x00000001);    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
	TIMER0_CTL_R |= 0x00000001;
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER0A timeout
  (*PeriodicTask0A)();                // execute user task
}



void (*PeriodicTask1A)(void);   // user function

// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1A_Init(void(*task)(void), unsigned long period){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  PeriodicTask1A = task;          // user function
  TIMER1_CTL_R &= ~(0x00000001);    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
	TIMER1_CTL_R |= 0x00000001;
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
  (*PeriodicTask1A)();                // execute user task
}

void Timer1_Enable()
{
  TIMER1_CTL_R |= 0x00000001;    // 10) enable TIMER1A
}
void Timer1_Disable()
{
  TIMER1_CTL_R &= ~(0x00000001);    
}

void (*PeriodicTask2A)(void);   // user function

// ***************** Timer2_Init ****************
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2A_Init(void(*task)(void), unsigned long period){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  PeriodicTask2A = task;          // user function
  TIMER2_CTL_R &= ~(0x00000001);    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
	TIMER2_CTL_R |= 0x00000001;
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
  (*PeriodicTask2A)();                // execute user task
}
void Timer2_Enable()
{
  TIMER2_CTL_R |= 0x00000001;    // 10) enable timer2A
}
void Timer2_Disable()
{
  TIMER2_CTL_R &= ~(0x00000001);    // 10) enable timer2A
}

void (*PeriodicTask3A)(void);   // user function

// ***************** Timer3_Init ****************
// Activate Timer3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer3A_Init(void(*task)(void), unsigned long period){
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  PeriodicTask3A = task;          // user function
  TIMER3_CTL_R &= ~(0x00000001);    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = period-1;    // 4) reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 51, interrupt number 35
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
	TIMER3_CTL_R |= 0x00000001;
}

void Timer3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER3A timeout
  (*PeriodicTask3A)();                // execute user task
}
void Timer3_Enable()
{
  TIMER3_CTL_R |= 0x00000001;    // 10) enable TIMER3A
}
void Timer3_Disable()
{
  TIMER3_CTL_R &= ~(0x00000001);    // 10) enable TIMER3A
}


void (*PeriodicTask4A)(void);   // user function

// ***************** Timer3_Init ****************
// Activate Timer4A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer4A_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x10;   // 0) activate TIMER3 --------------------------------------------------------------->
	PeriodicTask4A = task;          // user function
	TIMER4_CTL_R &= ~(0x00000001);    // 1) disable TIMER3A during setup
	TIMER4_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER4_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings ------------>
	TIMER4_TAILR_R = period-1;    // 4) reload value ------------------>
	TIMER4_TAPR_R = 0;            // 5) bus clock resolution ----------------->
	TIMER4_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
	TIMER4_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI17_R = (NVIC_PRI17_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 86, interrupt number 70
	NVIC_EN2_R = 1<<(70-64);      // 9) enable IRQ 35 in NVIC --------------------------------------------------------------->
	TIMER4_CTL_R |= 0x00000001;
}

void Timer4A_Handler(void){
	TIMER4_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER3A timeout
	(*PeriodicTask4A)();                // execute user task
}
void Timer4_Enable()
{
	TIMER4_CTL_R |= 0x00000001;    // 10) enable TIMER3A
}
void Timer4_Disable()
{
	TIMER4_CTL_R &= ~(0x00000001);    // 10) enable TIMER3A
}


void (*PeriodicTask5A)(void);   // user function

void Timer5A_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x20;   // 0) activate TIMER3 --------------------------------------------------------------->
	PeriodicTask5A = task;          // user function
	TIMER5_CTL_R &= ~(0x00000001);    // 1) disable TIMER3A during setup
	TIMER5_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER5_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings ------------>
	TIMER5_TAILR_R = period-1;    // 4) reload value ------------------>
	TIMER5_TAPR_R = 0;            // 5) bus clock resolution ----------------->
	TIMER5_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
	TIMER5_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI23_R = (NVIC_PRI23_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 108, interrupt number 92
	NVIC_EN2_R = 1<<(92-64);      // 9) enable IRQ 35 in NVIC --------------------------------------------------------------->
	TIMER5_CTL_R |= 0x00000001;
}

void Timer5A_Handler(void){
	TIMER5_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER3A timeout
	(*PeriodicTask5A)();                // execute user task
}
void Timer5_Enable()
{
	TIMER5_CTL_R |= 0x00000001;    
}
void Timer5_Disable()
{
	TIMER5_CTL_R &= ~(0x00000001);    
}
/*--------------------------------------------------------------------TIMER B---------------------------------------------------------------------------------------*/

void (*PeriodicTask0B)(void);   // user function

// ***************** Timer0_Init ****************
// Activate TIMER0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

void Timer0B_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
	PeriodicTask0B = task;          // user function
	TIMER0_CTL_R &= ~(0x00000010);    // 1) disable TIMER0A during setup
	TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER0_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	TIMER0_TBILR_R = period-1;    // 4) reload value
	TIMER0_TBPR_R = 0;            // 5) bus clock resolution
	TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
	TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 36, interrupt number 20
	NVIC_EN0_R = 1<<20;           // 9) enable IRQ 19 in NVIC
	TIMER0_CTL_R |= 0x00000010;
}

void Timer0B_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge TIMER0A timeout
	(*PeriodicTask0B)();                // execute user task
}



void (*PeriodicTask1B)(void);   // user function

// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1B_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
	PeriodicTask1B = task;          // user function
	TIMER1_CTL_R &= ~(0x00000010);    // 1) disable TIMER1A during setup
	TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER1_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	TIMER1_TBILR_R = period-1;    // 4) reload value
	TIMER1_TBPR_R = 0;            // 5) bus clock resolution
	TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
	TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 38, interrupt number 22
	NVIC_EN0_R = 1<<22;           // 9) enable IRQ 21 in NVIC
	TIMER1_CTL_R |= 0x00000010;
}

void Timer1B_Handler(void){
	TIMER1_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge TIMER1A timeout
	(*PeriodicTask1B)();                // execute user task
}


void (*PeriodicTask2B)(void);   // user function

// ***************** Timer2_Init ****************
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2B_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
	PeriodicTask2B = task;          // user function
	TIMER2_CTL_R &= ~(0x00000010);    // 1) disable timer2A during setup
	TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER2_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	TIMER2_TBILR_R = period-1;    // 4) reload value
	TIMER2_TBPR_R = 0;            // 5) bus clock resolution
	TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
	TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI6_R = (NVIC_PRI6_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 40, interrupt number 24
	NVIC_EN0_R = 1<<24;           // 9) enable IRQ 23 in NVIC
	TIMER2_CTL_R |= 0x00000010;
}

void Timer2B_Handler(void){
	TIMER2_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge TIMER2A timeout
	(*PeriodicTask2B)();                // execute user task
}


void (*PeriodicTask3B)(void);   // user function

// ***************** Timer3_Init ****************
// Activate Timer3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer3B_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
	PeriodicTask3B = task;          // user function
	TIMER3_CTL_R &= ~(0x00000010);    // 1) disable TIMER3A during setup
	TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER3_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
	TIMER3_TBILR_R = period-1;    // 4) reload value
	TIMER3_TBPR_R = 0;            // 5) bus clock resolution
	TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
	TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI9_R = (NVIC_PRI9_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 52, interrupt number 36
	NVIC_EN1_R = 1<<(36-32);      // 9) enable IRQ 35 in NVIC
	TIMER3_CTL_R |= 0x00000010;
}

void Timer3B_Handler(void){
	TIMER3_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge TIMER3A timeout
	(*PeriodicTask3B)();                // execute user task
}



void (*PeriodicTask4B)(void);   // user function

// ***************** Timer3_Init ****************
// Activate Timer3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer4B_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x10;   // 0) activate TIMER3 --------------------------------------------------------------->
	PeriodicTask4B = task;          // user function
	TIMER4_CTL_R &= ~(0x00000010);    // 1) disable TIMER3A during setup
	TIMER4_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER4_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings ------------>
	TIMER4_TBILR_R = period-1;    // 4) reload value ------------------>
	TIMER4_TBPR_R = 0;            // 5) bus clock resolution ----------------->
	TIMER4_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
	TIMER4_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI17_R = (NVIC_PRI17_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 87, interrupt number 71
	NVIC_EN2_R = 1<<(71-64);      // 9) enable IRQ 35 in NVIC --------------------------------------------------------------->
	TIMER4_CTL_R |= 0x00000010;
}

void Timer4B_Handler(void){
	TIMER4_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge TIMER3A timeout
	(*PeriodicTask4B)();                // execute user task
}


void (*PeriodicTask5B)(void);   // user function

void Timer5B_Init(void(*task)(void), unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x20;   // 0) activate TIMER3 --------------------------------------------------------------->
	PeriodicTask5B = task;          // user function
	TIMER5_CTL_R &= ~(0x00000010);    // 1) disable TIMER3A during setup
	TIMER5_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
	TIMER5_TBMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings ------------>
	TIMER5_TBILR_R = period-1;    // 4) reload value ------------------>
	TIMER5_TBPR_R = 0;            // 5) bus clock resolution ----------------->
	TIMER5_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
	TIMER5_IMR_R = 0x00000001;    // 7) arm timeout interrupt
	NVIC_PRI23_R = (NVIC_PRI23_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	// interrupts enabled in the main program after all devices initialized
	// vector number 109, interrupt number 93
	NVIC_EN2_R = 1<<(93-64);      // 9) enable IRQ 35 in NVIC --------------------------------------------------------------->
	TIMER5_CTL_R |= 0x00000010;
}

void Timer5B_Handler(void){
	TIMER5_ICR_R = TIMER_ICR_TBTOCINT;// acknowledge TIMER3A timeout
	(*PeriodicTask5B)();                // execute user task
}


void Timer0A_LoadValue(unsigned long period)
{
	TIMER0_TAILR_R = period-1;
}
void Timer1A_LoadValue(unsigned long period)
{
	TIMER1_TAILR_R = period-1;
}
void Timer2A_LoadValue(unsigned long period)
{
	TIMER2_TAILR_R = period-1;
}
void Timer3A_LoadValue(unsigned long period)
{
	TIMER3_TAILR_R = period-1;
}
void Timer4A_LoadValue(unsigned long period)
{
	TIMER4_TAILR_R = period-1;
}
void Timer5A_LoadValue(unsigned long period)
{
	TIMER5_TAILR_R = period-1;
}
/*
void Timer0B_LoadValue(unsigned long period)
{
	TIMER0_TBILR_R = period-1;
}
void Timer1B_LoadValue(unsigned long period)
{
	TIMER1_TBILR_R = period-1;
}
void Timer2B_LoadValue(unsigned long period)
{
	TIMER2_TBILR_R = period-1;
}
void Timer3B_LoadValue(unsigned long period)
{
	TIMER3_TBILR_R = period-1;
}
void Timer4B_LoadValue(unsigned long period)
{
	TIMER4_TBILR_R = period-1;
}
void Timer5B_LoadValue(unsigned long period)
{
	TIMER5_TBILR_R = period-1;
}*/