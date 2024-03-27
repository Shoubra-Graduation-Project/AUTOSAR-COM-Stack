#ifndef _COM_HELPINGFUNCTIONS_H_
#define _COM_HELPINGFUNCTIONS_H_

#include "../libraries/Std_Types.h"
#include "../include/Com_Types.h"
#include "../../Timer Driver/include/TIMERS_interface.h"

TIMERConfig_t *TIMERS_CONFG_STRUCT[12];

#define TIMER0 TIMERS_CONFG_STRUCT[0]
#define TIMER1 TIMERS_CONFG_STRUCT[1]
#define TIMER2 TIMERS_CONFG_STRUCT[2]
#define TIMER3 TIMERS_CONFG_STRUCT[3]
#define TIMER4 TIMERS_CONFG_STRUCT[4]
#define TIMER5 TIMERS_CONFG_STRUCT[5]
#define TIMER6 TIMERS_CONFG_STRUCT[6]
#define TIMER7 TIMERS_CONFG_STRUCT[7]
#define TIMER8 TIMERS_CONFG_STRUCT[8]
#define TIMER9 TIMERS_CONFG_STRUCT[9]
#define TIMER10 TIMERS_CONFG_STRUCT[10]
#define TIMER11 TIMERS_CONFG_STRUCT[11]

void InitAllTimers();
Std_ReturnType Com_InitPeriodicModeForIPdu(ComIPdu_type *IPdu);
Std_ReturnType FindAvailableTimer(ComIPdu_type *IPdu);
void AdjustTimerToIPdu(ComIPdu_type *IPdu);
void EnableIPduTimer(ComIPdu_type *IPdu);
void DisableIPduTimer(ComIPdu_type *IPdu);
boolean CheckPeriodicTimeFired(ComIPdu_type *IPdu);
void ClearPeriodicTimeFired(ComIPdu_type *IPdu);

#endif