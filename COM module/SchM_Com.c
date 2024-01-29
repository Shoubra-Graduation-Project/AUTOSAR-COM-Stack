
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







void Com_MainFunctionRx(void) {	
	uint16 pduId;
	//DEBUG_PRINT0(DEBUG_MEDIUM, "Com_MainFunctionRx() excecuting\n");
#if 0 	
	for (pduId = 0; !ComConfig->ComIPdu[pduId].Com_Arc_EOL; pduId++) {
#else
    for (pduId = 0; pduId < COM_N_IPDUS; pduId++) {
#endif	
		boolean pduUpdated = FALSE;
		const ComIPdu_type *IPdu = GET_IPdu(pduId);
		Com_Arc_IPdu_type *Arc_IPdu = GET_ArcIPdu(pduId);
		imask_t irq_state;
		uint16 i;
		Irq_Save(irq_state);
		for (i = 0; (IPdu->ComIPduSignalRef != NULL) && (IPdu->ComIPduSignalRef[i] != NULL); i++) 
        {
			const ComSignal_type *signal = IPdu->ComIPduSignalRef[i];
			Com_Arc_Signal_type * Arc_Signal = GET_ArcSignal(signal->ComHandleId);
			// Monitor signal reception deadline
			if ( (Arc_IPdu->Com_Arc_IpduStarted) && (signal->ComTimeoutFactor > 0) )
             {

				// Decrease deadline monitoring timer.
				timerDec(Arc_Signal->Com_Arc_DeadlineCounter);

				// Check if a timeout has occurred.
				if (Arc_Signal->Com_Arc_DeadlineCounter == 0)
                 {
					if (signal->ComRxDataTimeoutAction == COM_TIMEOUT_DATA_ACTION_REPLACE) {
						// Replace signal data.
						Arc_Signal->ComSignalUpdated = TRUE;
						Com_WriteSignalDataToPdu(signal->ComHandleId, signal->ComSignalInitValue);

					}

					// A timeout has occurred.
					if (signal->ComTimeoutNotification != NULL) {
						signal->ComTimeoutNotification();
					}

					// Restart timer
					Arc_Signal->Com_Arc_DeadlineCounter = signal->ComTimeoutFactor;
				}
			}

			if (Arc_Signal->ComSignalUpdated) {
				pduUpdated = TRUE;
			}
		}
		if (pduUpdated && IPdu->ComIPduSignalProcessing == DEFERRED && IPdu->ComIPduDirection == RECEIVE) {
			uint16 i;
			UnlockTpBuffer(getPduId(IPdu));
			memcpy(IPdu->ComIPduDeferredDataPtr,IPdu->ComIPduDataPtr,IPdu->ComIPduSize);
			for (i = 0; (IPdu->ComIPduSignalRef != NULL) && (IPdu->ComIPduSignalRef[i] != NULL); i++) {
				const ComSignal_type *signal = IPdu->ComIPduSignalRef[i];
				Com_Arc_Signal_type * Arc_Signal = GET_ArcSignal(signal->ComHandleId);
				if (Arc_Signal->ComSignalUpdated) {
					if (signal->ComNotification != NULL) {
						signal->ComNotification();
					}
					Arc_Signal->ComSignalUpdated = 0;
				}
			}
		}
		Irq_Restore(irq_state);
	}
}
