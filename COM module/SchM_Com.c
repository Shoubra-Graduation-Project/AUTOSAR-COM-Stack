
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
                Signal = IPdu->ComIPduSignalRef[ComMainRxSignalId];


                				
                if (Signal->ComSignalUpdated)
				{
                    /*[SWS_Com_00301] If ComIPduSignalProcessing for an I-PDU is configured to DEFERRED, 
                      the AUTOSAR COM module shall first copy the I-PDU’s data within the
                      Com_RxIndication function or the related TP reception functions respectively from 
                      the PduR into COM. Then the AUTOSAR COM module shall invoke the configured
                      ComNotifications for the included signals and signal groups asynchronously during
                      the next call to Com_MainFunctionRx*/
					if (signal->ComNotification != NULL)
					{
						signal->ComNotification();
					}
					Signal->ComSignalUpdated = FALSE;

                }

                if (Signal->ComTimeoutFactor > 0)
                {
                    timerDec(Signal->DeadlineMonitoringTimer);

                    if(Signal->DeadlineMonitoringTimer == 0)
                    {
                       switch(Signal->ComRxDataTimeoutAction )
                       { 
                             /* [SWS_Com_00470] If ComRxDataTimeoutAction is set to REPLACE, 
                             the AUTOSAR COM module shall replace the signal’s value by
                             its ComSignalInitValue when the reception deadline monitoring
                             timer of a signal expires*/
                            case REPLACE:
                            Com_WriteSignalDataToPdu(signal->ComHandleId, signal->ComSignalInitValue);
                            break;

                            /*[SWS_Com_00875] If ComRxDataTimeoutAction is set to SUBSTITUTE,
                            the AUTOSAR COM module shall replace the signal's value by 
                            its ComTimeoutSubstitutionValue when the reception deadline monitoring
                            timer of a signal expires*/
                            case SUBSTITUTE:
                            Com_WriteSignalDataToPdu(signal->ComHandleId, signal->ComTimeoutSubstitutionValue);
                            break;

                       }

                       if (Signal->ComTimeoutNotification != NULL)
                       {
						   Signal->ComTimeoutNotification();
					   }
                       
                       else
                       {

                       }
                       // Restart the timer
                       Signal->DeadlineMonitoringTimer = signal->ComTimeoutFactor;
                    }
                    else
                    {

                    }


                }
                else
                {

                }
            }

        }
        else
        {

        }

    }


}


/***************************************************************************************
 Service name:               Com_MainFunctionTx
 Service Id:                    0x19
 Parameters (in):               None
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        This function performs the processing of the AUTOSAR COM module's
                     transmission activities that are not directly handled within the 
                     COM's function invoked by the RTE, for example Com_SendSignal
 ***************************************************************************************/



void Com_MainFunctionTx(void)
{
    const ComIPdu_type *IPdu;


    uint8 ComMainTxPduId;
	uint8 ComMainTxSignalId;
    uint8 ComMainTxGroupSignalId;
    uint8 ComMainTxSignalGroupId;


    for ( pduId = 0; pduId<COM_NUM_OF_IPDU; pduId++)
    {
        IPdu = GET_IPdu(pduId);

        if(IPdu->ComIPduDirection == SEND)
        {
            switch(IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode)
            {
                case DIRECT:
                

                break;
            }

        }
        else
        {
           
        }
    }

}