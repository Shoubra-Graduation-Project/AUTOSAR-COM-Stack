#ifndef _COM_HELPINGFUNCTIONS_H_
#define _COM_HELPINGFUNCTIONS_H_

#include "../libraries/Std_Types.h"
#include "../include/Com_Types.h"
#include "../../Timer Driver/include/Timer.h"




void InitAllTimers();
uint8 Com_InitPeriodicModeForIPdu(ComIPdu_type *IPdu);
uint8 FindAvailableTimer(ComIPdu_type *IPdu);
void AdjustTimerToIPdu(ComIPdu_type *IPdu);
void EnableIPduTimer(ComIPdu_type *IPdu);
void DisableIPduTimer(ComIPdu_type *IPdu);
boolean CheckPeriodicTimeFired(ComIPdu_type *IPdu);
void ClearPeriodicTimeFired(ComIPdu_type *IPdu);
#endif