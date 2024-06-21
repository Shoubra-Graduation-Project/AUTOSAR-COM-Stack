

#include "../Common/Std_Types.h"
#include "include/Com.h"
#include "include/Com_Cfg.h"
#include "./PLL.h"
#include "./include/PeriodicMode_HelpingFUnctions.h"

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
const long COLORWHEEL[WHEELSIZE] = {RED, RED+GREEN, GREEN, GREEN+BLUE, BLUE, BLUE+RED, RED+GREEN+BLUE, 0};
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

void UserTask(void){
  static int i = 0;
  LEDS = COLORWHEEL[i&(WHEELSIZE-1)];
  i = i + 1;
}


//debug code
int x = 5;
int main(void){
	/*ComTxMode_type mode1_struct;
	ComTxMode_type * mode1 = & mode1_struct;
	
	ComTxMode_type mode2_struct;
	ComTxMode_type * mode2 = & mode2_struct;
	
	ComTxModeTrue_type true_struct;
	ComTxModeTrue_type * True = &true_struct;
	
	ComTxModeFalse_type false_struct;
	ComTxModeFalse_type * False = &false_struct;

	ComTxIPdu_type txPdu_struct;
	ComTxIPdu_type * txPdu = &txPdu_struct;
	
	ComTxIPdu_type txPdu2_struct;
	ComTxIPdu_type * txPdu2 = &txPdu2_struct;
	
	ComIPdu_type pdu_struct;
	ComIPdu_type * pdu = &pdu_struct;
	
	ComIPdu_type pdu2_struct;
	ComIPdu_type * pdu2 = &pdu2_struct;
		
	mode1_struct.ComTxModeTimePeriod = 1000;
	mode2_struct.ComTxModeTimePeriod = 1000;
	true_struct.ComTxMode = mode1;
	false_struct.ComTxMode = mode2;
	txPdu_struct.ComCurrentTransmissionSelection = 1;
	//txPdu_struct.ComTxTimerID = TIMERS_TIMER0A;
	//txPdu_struct.ComTxTimerBlock = 'A';
	//txPdu_struct.ComTxTimerNumber = 0;
	txPdu_struct.ComTxModeTrue = True;
	pdu_struct.ComTxIPdu = txPdu;
	
	txPdu2_struct.ComCurrentTransmissionSelection = 0;
	//txPdu2_struct.ComTxTimerID = TIMERS_TIMER0A;
	//txPdu2_struct.ComTxTimerBlock = 'A';
	//txPdu2_struct.ComTxTimerNumber = 0;
	txPdu2_struct.ComTxModeFalse = False;
	pdu2_struct.ComTxIPdu = txPdu2;*/
	
	volatile unsigned long delay;
	ComIPdu_type * pdu = ComConfig.ComIPdu[0];
	ComIPdu_type * pdu2 = ComConfig.ComIPdu[1];
	
  PLL_Init();                      // bus clock at 80 MHz
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // activate port F
  delay = SYSCTL_RCGC2_R;          // allow time to finish activating
  GPIO_PORTF_DIR_R |= 0x0E;        // make PF3-1 output (PF3-1 built-in LEDs)
  GPIO_PORTF_AFSEL_R &= ~0x0E;     // disable alt funct on PF3-1
  GPIO_PORTF_DEN_R |= 0x0E;        // enable digital I/O on PF3-1
                                   // configure PF3-1 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
  LEDS = 0;                        // turn all LEDs off
	
	
  EnableInterrupts();
	Com_InitPeriodicModeForIPdu(pdu);
	x = 10;
	Com_InitPeriodicModeForIPdu(pdu2);
	x = 15;
  while(1){
    
  }
}
