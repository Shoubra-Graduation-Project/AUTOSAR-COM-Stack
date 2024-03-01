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
 Service name:               Com_MainFunctionRx
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
        ComIPdu_type *IPdu = NULL;
        ComIPduGroup_type * IPduGroup; 

        uint8 ComMainRxPduId;
    
    // Loop over all PDUs
	for ( ComMainRxPduId = 0; ComMainRxPduId < COM_NUM_OF_IPDU; ComMainRxPduId++)
	{
        // Get IPDU
	   	IPdu = GET_IPdu(ComMainRxPduId);

        if(IPdu != NULL)
        {
            // If the Current IPD-U belongs to any IPDU group
            if(IPdu->ComIPduGroupRef != NULL)
            {
               IPduGroup =  IPdu->ComIPduGroupRef;

               // If the IPD-U group is started
               if(IPduGroup->IpduGroupFlag == STARTED)
               {
                
                // Proceed to process this recieve IPDU
                CheckRXIpdu(IPdu);

               }
               else
               {
                 /* IPDU group STOPPED*/
               }

                
            }
            else
            // IPD-U does not belong to any IPDU group
            {
                CheckRXIpdu(IPdu);
            }     
           
        }
        else
        {
            /* IPDU is NULL */

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
void Com_MainFunctionTx (void)
{
	for(uint8 currentIPduID = 0; currentIPduID<COM_NUM_OF_IPDU; currentIPduID++)
	{
		ComIPdu_type* IPdu = GET_IPDU(currentIPduID);
		if(IPdu !=NULL)
		{
			ComIPduGroup_type * IPduGroup =  IPdu->ComIPduGroupRef;
			if((IPduGroup == NULL || IPduGroup->IpduGroupFlag == STARTED) && IPdu->ComIPduDirection == SEND)
			{	
				/*-----------------------------------------------Notify RTE layer if IPDU is DEFERED--------------------------------------------------------*/
				if(IPdu->ComIPduSignalProcessing == DEFERRED && (IPdu->ComTxIPdu).ComIsIPduDeferred == 1)
				{
					for(uint16 signalID=0; (IPdu->ComIPduSignalRef[signalID] != NULL); signalID++)
					{
						if(IPdu->ComIPduSignalRef[signalID]->ComTimeoutNotification != NULL)
						{
							IPdu->ComIPduSignalRef[signalID]->ComTimeoutNotification();
						}
					}
					for(uint16 signalGroupID=0; (IPdu->ComIPduSignalGroupRef[signalGroupID] != NULL); signalGroupID++)
					{
						if(IPdu->ComIPduSignalGroupRef[signalGroupID]->ComTimeoutNotification != NULL)
						{
							IPdu->ComIPduSignalGroupRef[signalGroupID]->ComTimeoutNotification();
						}
					}
					(IPdu->ComTxIPdu).ComIsIPduDeferred = 0;
				}
				
				/*---------------------------------------------------------Start sending IPDU-------------------------------------------------------------------*/
				if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 1)
				{
					switch(IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode)
					{
						case MIXED:
								if(IPdu->ComTxIPdu.ComFirstPeriodicModeEntry == 1)
								{
									IPdu->ComTxIPdu.ComFirstPeriodicModeEntry = 0;
									com_packSignalsToPdu(IPdu);
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
									if((IPdu->ComTxIPdu.ComNumberOfTransmissions) == 0)
									{
										if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
									}
										
								}
								if(IPdu->ComTxIPdu.ComNumberOfTransmissions > 0)
								{
									com_packSignalsToPdu(IPdu);
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									while( (IPdu->ComTxIPdu.ComNumberOfTransmissions) > 0)
									{
										Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
										IPdu->ComTxIPdu.ComNumberOfTransmissions--;
										delay(IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeRepetitionPeriod);
										if((IPdu->ComTxIPdu.ComNumberOfTransmissions) == 0)
										{
											if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
										}
									}
									}
									
						case PERIODIC:
								if(IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.comPeriodicTimeFired == 1)
								{
									IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.comPeriodicTimeFired == 0;
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
									if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
								}
								break;
						case DIRECT:
								if(IPdu->ComTxIPdu.ComNumberOfTransmissions > 0)
								{
									com_packSignalsToPdu(IPdu);
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									while( IPdu->ComTxIPdu.ComNumberOfTransmissions > 0)
									{
										Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
										IPdu->ComTxIPdu.ComNumberOfTransmissions--;
										delay(IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeRepetitionPeriod);
										if((IPdu->ComTxIPdu.ComNumberOfTransmissions) == 0)
										{
											if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
										}
									}
								}
					
					}
				}
				else if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 0)
				{
					switch(IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode)
					{
						case MIXED:
								if(IPdu->ComTxIPdu.ComFirstPeriodicModeEntry == 1)
								{
									IPdu->ComTxIPdu.ComFirstPeriodicModeEntry = 0;
									com_packSignalsToPdu(IPdu);
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
									if((IPdu->ComTxIPdu.ComNumberOfTransmissions) == 0)
									{
										if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
									}
								
								}
								if(IPdu->ComTxIPdu.ComNumberOfTransmissions > 0)
								{
									com_packSignalsToPdu(IPdu);
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									while( (IPdu->ComTxIPdu.ComNumberOfTransmissions) > 0)
									{
										Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
										IPdu->ComTxIPdu.ComNumberOfTransmissions--;
										delay(IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeRepetitionPeriod);
										if((IPdu->ComTxIPdu.ComNumberOfTransmissions) == 0)
										{
											if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
										}
									}
								}
									
						case PERIODIC:
								if(IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.comPeriodicTimeFired == 1)
								{
									IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.comPeriodicTimeFired == 0;
									com_packSignalsToPdu(IPdu);
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
									if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
								}
								break;
						case DIRECT:
								if(IPdu->ComTxIPdu.ComNumberOfTransmissions > 0)
								{
									com_packSignalsToPdu(IPdu);
									if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout();}
									while( IPdu->ComTxIPdu.ComNumberOfTransmissions > 0)
									{
										Com_TriggerIPDUSend(IPdu->ComIPduHandleId);
										IPdu->ComTxIPdu.ComNumberOfTransmissions--;
										delay(IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeRepetitionPeriod);
										if((IPdu->ComTxIPdu.ComNumberOfTransmissions) == 0)
										{
											if(ComTxIPdu.ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu.ComMinimumDelayTime);}
										}
									}
								}
									
					}
				}
			}
			
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



/***************************************************************************************
 Service name:               CheckRXIpdu
 Parameters (in):               IPdu
 Parameters (inout):            None
 Parameters (out):              None
 Return value:                  None
 Description:        This function checks the recived IPDUs and loop over all signals
                     and signal groups for each IPDU to process Com_MainFunctionRX 
                     Requirements
 ***************************************************************************************/
void CheckRXIpdu(ComIPdu_type IPdu)
{
        
	const ComSignal_type *signal = NULL;
    const ComSignalGroup_type *SignalGroup =NULL;
        

	uint8 ComMainRxSignalId;
    uint8 ComMainRxSignalGroupId;

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
