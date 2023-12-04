
#ifndef _SchM_COM_H_
#define _SchM_COM_H_/*------------------------------------------Includes Start---------------------------------------------------------------------*/
#include "Std_Types.h"
/*------------------------------------------Includes End-----------------------------------------------------------------------*//*------------------------------------------Software Interfaces Declarations Start---------------------------------------------*//**   /brief    Main function for Reception  /details  This function performs the processing of the AUTOSAR COM module's receive processing that are not directly handled within the COM's functions 
			invoked by the PDU-R, for example Com_RxIndication.  /param	None  /return	None */void Com_MainFunctionRx (void);

/**   /brief    Main function for Transmission  /details  This function performs the processing of the AUTOSAR COM module's transmission activities that are not directly handled within the COM's function 
			invoked by the RTE, for example Com_SendSignal.  /param	None  /return	None */void Com_MainFunctionTx (void);/*------------------------------------------Software Interfaces Declarations End---------------------------------------------*/#endif