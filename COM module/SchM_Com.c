/*********************************************************************************
 *                                    Includes                                   *
 ********************************************************************************/


#include "include/Com.h"
#include "include/Com_Types.h"
#include "include/Com_Cfg.h"
#include "include/ComMacros.h"
#include "include/com_buffers.h"
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
        const ComSignalGroup_type *SignalGroup =NULL;

        uint8 ComMainRxPduId;
		uint8 ComMainRxSignalId;
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
		
                Com_MainFunctionRxSignal(Signal);
                CopySignalfromBGtoFG(ComMainRxSignalId);
               
            }

            for (ComMainRxSignalGroupId = 0; IPdu[ComMainRxSignalGroupId].ComIPduSignalGroupRef[ComMainRxSignalGroupId] != NULL; ComMainRxSignalGroupId++)
            {
                SignalGroup = IPdu->ComIPduSignalGroupRef[ComMainRxSignalGroupId];

                Com_MainFunctionRxSignalGroup(SignalGroup);

                CopySignalGroupfromBGtoSB(ComMainRxSignalGroupId);

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

/***************************************************************************************
 Service name:               Com_MainFunctionRxSignal
 Parameters (in):               Signal
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        This function performs the processing of the AUTOSAR COM module's
                     checks for recieved signals in Com_MainFunctionRx API
 ***************************************************************************************/

void Com_MainFunctionRxSignal(ComSignal_type Signal)
{
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

    if (Signal->ComTimeout > 0)
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
                memcpy((uint8*)Signal->ComFGBuffer, (uint8*)Signal->ComSignalInitValue,(Signal->ComBitSize)/8);
                break;

                /*[SWS_Com_00875] If ComRxDataTimeoutAction is set to SUBSTITUTE,
                the AUTOSAR COM module shall replace the signal's value by 
                its ComTimeoutSubstitutionValue when the reception deadline monitoring
                timer of a signal expires*/
                case SUBSTITUTE:
                memcpy((uint8*)Signal->ComFGBuffer, (uint8*)Signal->ComTimeoutSubstitutionValue,(Signal->ComBitSize)/8);
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
            Signal->DeadlineMonitoringTimer = signal->ComTimeout;
        }
        else
        {
        }

    }
    else
    {
    }
}

/***************************************************************************************
 Service name:               Com_MainFunctionRxSignalGroup
 Parameters (in):               SignalGroup
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        This function performs the processing of the AUTOSAR COM module's
                     checks for recieved signal groups in Com_MainFunctionRx API
 ***************************************************************************************/
void Com_MainFunctionRxSignalGroup(ComSignalGroup_type SignalGroup)
{
    uint8 ComMainRxGroupSignalId;
    
    if (SignalGroup->ComTimeout > 0)
    {
        timerDec(SignalGroup->DeadlineMonitoringTimer);

        if(SignalGroup->DeadlineMonitoringTimer == 0)
        {
            for(ComMainRxGroupSignalId=0 ;(SignalGroup->ComGroupSignal != NULL && SignalGroup->ComGroupSignal[ComMainRxGroupSignalId] != NULL) ; ComMainRxGroupSignalId++)
            {
                GroupSignal = SignalGroup->ComGroupSignal[ComMainRxGroupSignalId];

                switch(GroupSignal->ComRxDataTimeoutAction )
                { 
                    /* [SWS_Com_00470] If ComRxDataTimeoutAction is set to REPLACE, 
                    the AUTOSAR COM module shall replace the signal’s value by
                    its ComSignalInitValue when the reception deadline monitoring
                    timer of a signal expires*/
                    case REPLACE:
                    memcpy((uint8*)SignalGroup->ComShadowBuffer, (uint8*)GroupSignal->ComSignalInitValue,(GroupSignal->ComBitSize)/8);

                    //Com_WriteSignalDataToPdu(GroupSignal->ComHandleId, GroupSignal->ComSignalInitValue);
                    break;

                    /*[SWS_Com_00875] If ComRxDataTimeoutAction is set to SUBSTITUTE,
                    the AUTOSAR COM module shall replace the signal's value by 
                    its ComTimeoutSubstitutionValue when the reception deadline monitoring
                    timer of a signal expires*/
                    case SUBSTITUTE:
                    memcpy((uint8*)SignalGroup->ComShadowBuffer, (uint8*)GroupSignal->ComTimeoutSubstitutionValue,(GroupSignal->ComBitSize)/8);
                    //Com_WriteSignalDataToPdu(GroupSignal->ComHandleId, GroupSignal->ComTimeoutSubstitutionValue);
                    break;

                }

            }

            if (SignalGroup->ComTimeoutNotification != NULL)
            {
				SignalGroup->ComTimeoutNotification();
		    }
            else
            {

            }

            // Restart the timer
            SignalGroup->DeadlineMonitoringTimer = SignalGroup->ComTimeout;

        }

    }
    else
    {

    }

}




