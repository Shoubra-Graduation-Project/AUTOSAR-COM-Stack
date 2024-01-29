
/*********************************************************************************
 *                                    Includes                                   *
 ********************************************************************************/


#include "include/Com.h"
#include "include/Com_Types.h"
#include "include/Com_Cfg.h"
#include "include/ComMacros.h"
#include <cstddef>


#define timerDec(timer)                         
	if (timer > 0) {     
                               
		timer = timer - 1;                      
	}

/**********************************************************************************
 *                             Functions Definitions                              *
 **********************************************************************************/


/**********************************************************************************
 Service name:               Com_MainFunctionTx
 Service Id:                    0x19
 Parameters (in):               None
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:    This function performs the processing of the AUTOSAR COM module's 
                 transmission activities that are not directly handled within 
                 the COM's function invoked by the RTE, for example Com_SendSignal
 ***********************************************************************************/
void Com_MainFunctionRx(void)
{
    	const ComIPdu_type *IPdu = NULL;
	    const ComSignal_type *signal = NULL;


        uint8 ComMainRxPduId;
		uint8 ComMainRxSignalId;
        uint8 ComMainRxGroupSignalId;
        uint8 ComMainRxSignalGroupId;

    
    // Loop over all PDUs
	for ( ComMainRxPduId = 0; ComMainRxPduId < COM_NUM_OF_IPDU; ComMainRxPduId++)
	{
        // Get IPDU
	   	IPdu = GET_IPdu(ComMainRxPduId);

        if (IPdu->ComIPduSignalProcessing == DEFERRED && IPdu->ComIPduDirection == RECEIVE)
        {
            // For each signal at PDU
            for ( ComMainRxSignalId = 0; (IPdu->ComIPduSignalRef[ComMainRxSignalId] != NULL); ComMainRxSignalId++)
            {
                //Get signal
                Signal = IPdu->ComIPduSignalRef[ComMainRxPduId];

                if (signal->ComTimeoutFactor > 0)
                {
                    timerDec(Signal->DeadlineCounter);


                }
            }

        }

    }


}



