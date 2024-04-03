/*
time structs for confg
array of busy timers
array of is timer fired

function to adjust timer struct to ipdu confg
function to check if ipdu timer is fired
*/

#include "../../Timer Driver/include/Timer.h"
#include "../libraries/Std_Types.h"
#include "../include/PeriodicMode_HelpingFUnctions.h"
#include "../include/Com_Types.h"

boolean TIMER_BUSY[2][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}};
boolean TIMER_FIRED[2][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}};

void HandleTimer_0A()
{
	TIMER_FIRED[0][0] = 1;
}

void HandleTimer_1A()
{
	TIMER_FIRED[0][1] = 1;
}

void HandleTimer_2A()
{
	TIMER_FIRED[0][2] = 1;
}

void HandleTimer_3A()
{
	TIMER_FIRED[0][3] = 1;
}

void HandleTimer_4A()
{
	TIMER_FIRED[0][4] = 1;
}

void HandleTimer_5A()
{
	TIMER_FIRED[0][5] = 1;
}

void HandleTimer_0B()
{
	TIMER_FIRED[1][0] = 1;
}

void HandleTimer_1B()
{
	TIMER_FIRED[1][1] = 1;
}

void HandleTimer_2B()
{
	TIMER_FIRED[1][2] = 1;
}

void HandleTimer_3B()
{
	TIMER_FIRED[1][3] = 1;
}

void HandleTimer_4B()
{
	TIMER_FIRED[1][4] = 1;
}

void HandleTimer_5B()
{
	TIMER_FIRED[1][5] = 1;
}


void InitAllTimers()
{
	Timer0A_Init(HandleTimer_0A, 0);
	Timer1A_Init(HandleTimer_1A, 0);
	Timer2A_Init(HandleTimer_2A, 0);
	Timer3A_Init(HandleTimer_3A, 0);
	Timer4A_Init(HandleTimer_4A, 0);
	Timer5A_Init(HandleTimer_5A, 0);
	Timer0B_Init(HandleTimer_0B, 0);
	Timer1B_Init(HandleTimer_1B, 0);
	Timer2B_Init(HandleTimer_2B, 0);
	Timer3B_Init(HandleTimer_3B, 0);
	Timer4B_Init(HandleTimer_4B, 0);
	Timer5B_Init(HandleTimer_5B, 0);
}

//call when an IPdu enters Periodic mode
Std_ReturnType Com_InitPeriodicModeForIPdu(ComIPdu_type *IPdu)
{
	Std_ReturnType returnVlaue = E_OK;
	if(FindAvailableTimer(IPdu) == E_OK)
	{
		AdjustTimerToIPdu(IPdu);
		EnableIPduTimer(IPdu);
	}
	else
	{
		returnVlaue = E_NOT_OK;
	}
	
	return returnVlaue;
}

Std_ReturnType FindAvailableTimer(ComIPdu_type *IPdu)
{
	Std_ReturnType returnVlaue = E_OK;
	uint8 i, j, found=0;
	for(i = 0; i<2; i++)
	{
		for(j = 0; j<6; j++)
		{
			if(TIMER_BUSY[i][j] == 0)
			{
				found=1;
				break;
			}
		}
		if(found)
		{
			break;
		}
	}
	
	if(found)
	{
		TIMER_BUSY[i][j] = 0;
		switch(j)
		{
			case 0:
					IPdu->ComTxIPdu->ComTxTimerNumber = j;
					switch(i)
					{
						case 0:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER0A;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'A';
							break;
						case 1:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER0B;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'B';
					}
					break;
			case 1:
					IPdu->ComTxIPdu->ComTxTimerNumber = j;
					switch(i)
					{
						case 0:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER1A;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'A';
							break;
						case 1:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER1B;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'B';
					}
					break;
			case 2:
					IPdu->ComTxIPdu->ComTxTimerNumber = j;
					switch(i)
					{
						case 0:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER2A;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'A';
							break;
						case 1:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER2B;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'B';
					}
					break;
			case 3:
					IPdu->ComTxIPdu->ComTxTimerNumber = j;
					switch(i)
					{
						case 0:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER3A;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'A';
							break;
						case 1:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER3B;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'B';
					}
					break;
			case 4:
					IPdu->ComTxIPdu->ComTxTimerNumber = j;
					switch(i)
					{
						case 0:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER0A;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'A';
							break;
						case 1:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER0B;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'B';
					}
					break;
			case 5:
					IPdu->ComTxIPdu->ComTxTimerNumber = j;
					switch(i)
					{
						case 0:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER5A;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'A';
							break;
						case 1:
							IPdu->ComTxIPdu->ComTxTimerID = TIMERS_TIMER5B;
							IPdu->ComTxIPdu->ComTxTimerBlock = 'B';
					}
					break;	
		}
	}
	else
	{
		returnVlaue = E_NOT_OK;
	}
	
	return returnVlaue;
}

void AdjustTimerToIPdu(ComIPdu_type *IPdu)
{
	uint32 loadValue;
	if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 0)
	{
		loadValue = IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeTimePeriod;
	}
	else
	{
		loadValue = IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeTimePeriod;
	}
	
	if(IPdu->ComTxIPdu->ComTxTimerBlock == 'A')
	{
		switch(IPdu->ComTxIPdu->ComTxTimerNumber)
		{
			case 0: Timer0A_LoadValue(loadValue);
							break;
			case 1: Timer1A_LoadValue(loadValue);
							break;
			case 2: Timer2A_LoadValue(loadValue);
							break;
			case 3: Timer3A_LoadValue(loadValue);
							break;
			case 4: Timer4A_LoadValue(loadValue);
							break;
			case 5: Timer5A_LoadValue(loadValue);
							break;
		}
	}
	else if(IPdu->ComTxIPdu->ComTxTimerBlock == 'B')
	{
		switch(IPdu->ComTxIPdu->ComTxTimerNumber)
		{
			case 0: Timer0B_LoadValue(loadValue);
							break;
			case 1: Timer1B_LoadValue(loadValue);
							break;
			case 2: Timer2B_LoadValue(loadValue);
							break;
			case 3: Timer3B_LoadValue(loadValue);
							break;
			case 4: Timer4B_LoadValue(loadValue);
							break;
			case 5: Timer5B_LoadValue(loadValue);
							break;
		}
	}
	else{}
}

void EnableIPduTimer(ComIPdu_type *IPdu)
{
	switch(IPdu->ComTxIPdu->ComTxTimerNumber)
	{
		case 0: Timer0_Enable();
							break;
		case 1: Timer1_Enable();
							break;
		case 2: Timer2_Enable();
							break;
		case 3: Timer3_Enable();
							break;
		case 4: Timer4_Enable();
							break;
		case 5: Timer5_Enable();
							break;
	}
}

void DisableIPduTimer(ComIPdu_type *IPdu)
{
	uint8 i = IPdu->ComTxIPdu->ComTxTimerBlock, j = IPdu->ComTxIPdu->ComTxTimerNumber;
	TIMER_BUSY[i][j] = 0;
	switch(IPdu->ComTxIPdu->ComTxTimerNumber)
	{
		case 0: Timer0_Disable();
							break;
		case 1: Timer1_Disable();
							break;
		case 2: Timer2_Disable();
							break;
		case 3: Timer3_Disable();
							break;
		case 4: Timer4_Disable();
							break;
		case 5: Timer5_Disable();
							break;
	}
}


boolean CheckPeriodicTimeFired(ComIPdu_type *IPdu)
{
	boolean returnVlaue;
	uint8 i = IPdu->ComTxIPdu->ComTxTimerBlock, j = IPdu->ComTxIPdu->ComTxTimerNumber;
	returnVlaue = TIMER_FIRED[i][j];
	
	return returnVlaue;
}

void ClearPeriodicTimeFired(ComIPdu_type *IPdu)
{
	uint8 i = IPdu->ComTxIPdu->ComTxTimerBlock, j = IPdu->ComTxIPdu->ComTxTimerNumber;
	TIMER_FIRED[i][j] = 0;
}


