
#ifndef INCLUDE_SCHM_COM_H_
#define INCLUDE_SCHM_COM_H_
/*********************************************************************************************************************************
 Service name:               Com_MainFunctionTx
 Service Id:                    0x19
 Parameters (in):               None
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        This function performs the processing of the AUTOSAR COM module's transmission activities
                     that are not directly handled within the COM's function invoked by the RTE,
 *******************************************************************************************************************************/
#include "../include/Com_Types.h"
void Com_MainFunctionTx(void);
void CheckRXIpdu(ComIPdu_type IPdu);
#endif

