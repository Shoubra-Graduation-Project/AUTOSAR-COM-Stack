/*
time structs for confg
array of busy timers
array of is timer fired

function to adjust timer struct to ipdu confg
function to check if ipdu timer is fired
*/

#include "../../Timer Driver/include/TIMERS_interface.h"
#include "../libraries/Std_Types.h"
#include "../include/PeriodicMode_HelpingFUnctions.h"
#include "../include/Com_Types.h"

boolean TIMER_BUSY[2][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}};
boolean TIMER_FIRED[2][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}};

void InitAllTimers()
{
	uint8 i;
	for(i = 0; i<12; i++)
	{
		TIMERS_vidInit(TIMERS_CONFG_STRUCT[i]);
	}
	
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
	float32 loadValue;
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
		TIMERS_vidSetLoadValueA(IPdu->ComTxIPdu->ComTxTimerID, loadValue);
	}
	else if(IPdu->ComTxIPdu->ComTxTimerBlock == 'B')
	{
		TIMERS_vidSetLoadValueB(IPdu->ComTxIPdu->ComTxTimerID, loadValue);
	}
	else{}
}

void EnableIPduTimer(ComIPdu_type *IPdu)
{
	TIMERS_vidEnableTimer(IPdu->ComTxIPdu->ComTxTimerID, IPdu->ComTxIPdu->ComTxTimerBlock);
}

void DisableIPduTimer(ComIPdu_type *IPdu)
{
	uint8 i = IPdu->ComTxIPdu->ComTxTimerBlock, j = IPdu->ComTxIPdu->ComTxTimerNumber;
	TIMER_BUSY[i][j] = 0;
	TIMERS_vidDisableTimer(IPdu->ComTxIPdu->ComTxTimerID, IPdu->ComTxIPdu->ComTxTimerBlock);
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

void WriteHandlers()
{
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_0A);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_1A);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_2A);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_3A);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_4A);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_5A);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_0B);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_1B);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_2B);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_3B);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_4B);
	TIMERS_vidPutFunction(TIMERS_TIMER0A, HandleTimer_5B);
}

