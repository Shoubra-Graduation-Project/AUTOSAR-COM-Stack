#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include "../COM module/include/Com_Types.h"
#include "../COM module/libraries/Std_Types.h"
#include "../COM module/include/Com.h"

Std_ReturnType PduR_ComTransmit(Std_ReturnType TransmisionReturnValue, PduIdType PduID, const PduInfoType* PduInfo);

#endif