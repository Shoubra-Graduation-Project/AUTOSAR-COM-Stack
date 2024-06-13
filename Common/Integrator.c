
#include "../COM module/include/Com_Types.h"
#include "Std_Types.h"
#include "../COM module/include/Com.h"
#include "../CanIf Module/inc/CanIf.h"

Std_ReturnType PduR_ComTransmit(Std_ReturnType TransmisionReturnValue, PduIdType PduID, const PduInfoType* PduInfo)
{
	Std_ReturnType retrunvalue = CanIf_Transmit(PduID, PduInfo);
	return retrunvalue;
}