

#include "../Common/Std_Types.h"
#include "include/Com.h"
#include "include/Com_Cfg.h"
#include "./PLL.h"
#include "./include/PeriodicMode_HelpingFunctions.h"
#include "./include/Com_HelpingFunctions.h"
#include "./include/SchM_Com.h"
#include "../Common/integrator.h"
#include "../CanIf Module/inc/CanIf_cfg.h"

#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define PF1       (*((volatile unsigned long *)0x40025008))
#define PF2       (*((volatile unsigned long *)0x40025010))
#define PF3       (*((volatile unsigned long *)0x40025020))
#define LEDS      (*((volatile unsigned long *)0x40025038))
#define RED       0x02
#define BLUE      0x04
#define GREEN     0x08
#define WHEELSIZE 8           // must be an integer multiple of 2
                              //    red, yellow,    green, light blue, blue, purple,   white,          dark
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode




//debug code

int main(void){
	
	ComIPdu_type * IPdu = ComConfig.ComIPdu[0];
	volatile unsigned long delay;
	int returnVal= 0;
	int x = 5, y = 0, z = 0, m = 0;
  PLL_Init();                      // bus clock at 80 MHz
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // activate port F
  delay = SYSCTL_RCGC2_R;          // allow time to finish activating
  GPIO_PORTF_DIR_R |= 0x0E;        // make PF3-1 output (PF3-1 built-in LEDs)
  GPIO_PORTF_AFSEL_R &= ~0x0E;     // disable alt funct on PF3-1
  GPIO_PORTF_DEN_R |= 0x0E;        // enable digital I/O on PF3-1
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
  EnableInterrupts();
	
	/*-------------------------------Start COM Send Senario-----------------------------------*/
	Com_Init(&ComConfig);
	Com_SendSignal(32768, &x);
	Com_InvalidateSignal(32769);
	Com_SendSignal(0, &x);
	Com_SendSignalGroup(0);
	com_packSignalsToPdu(IPdu);
	IPdu->ComIPduCounter->ComCurrentCounterValue = IPdu->ComIPduCounter->ComCurrentCounterValue + 1;
	Com_writeCounterValueToPduBuffer(IPdu, (uint8)IPdu->ComIPduCounter->ComCurrentCounterValue);
	CanIf_Init(&CanIf);
	PduR_ComTransmit(E_OK, IPdu->ComIPduHandleId, IPdu->PduInfo);
	/*-------------------------------Start COM Recieve Senario-----------------------------------*/
	/*Com_RxIndication(2, IPdu->PduInfo);
	Com_MainFunctionRx();
	Com_ReceiveSignal(32772, &y);
	Com_ReceiveSignal(32773, &z);
	Com_ReceiveSignalGroup(2);
	Com_ReceiveSignal(2, &m);*/
  while(1){}
}
