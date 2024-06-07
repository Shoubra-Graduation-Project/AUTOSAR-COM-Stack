/*********************************************************************************
 *                                    Includes                                   *
 ********************************************************************************/


#include "../include/Com.h"
#include "../include/SchM_Com.h"
#include "../include/Com_Types.h"
#include "../include/Com_Cfg.h"
#include "../include/ComMacros.h"
#include "../include/com_buffers.h"
#include "../include/Com_HelpingFunctions.h"
#include "../include/PeriodicMode_HelpingFunctions.h"
#include "../libraries/Std_Types.h"
#include "../../Common/Integrator.h"
#include <stdio.h>






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
    ComIPduGroup_type *IPduGroup = NULL; // Initialize IPduGroup

    uint8 ComMainRxPduId;
    
    // Loop over all PDUs
    for (ComMainRxPduId = 0; ComMainRxPduId < COM_NUM_OF_IPDU; ComMainRxPduId++)
    {
        // Get IPDU
        IPdu = GET_IPDU(ComMainRxPduId);

        if (IPdu != NULL)
        {
            // If the Current IPDU belongs to any IPDU group
            if (IPdu->ComIPduGroupRef == NULL || IPdu->ComIPduGroupRef->IpduGroupFlag == STARTED)
            {
                CheckRXIpdu(*IPdu);
            }
            else{}
        }
        else{}
    }
}

void DirectModeSequence(ComIPdu_type *IPdu)
{
	if(IPdu->ComTxIPdu->ComNumberOfTransmissions > 0)
	{
			com_packSignalsToPdu(IPdu);
			if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout(*(IPdu->ComPduIdRef), IPdu->PduInfo);}
			else{}
			while( (IPdu->ComTxIPdu->ComNumberOfTransmissions) > 0)
			{
				uint8 TransmisionReturnValue = E_OK;
				Com_writeCounterValueToPduBuffer(IPdu, (IPdu->ComIPduCounter)->ComCurrentCounterValue);
				IPdu->ComIPduCounter->ComCurrentCounterValue = (IPdu->ComIPduCounter->ComCurrentCounterValue + 1)%(power(IPdu->ComIPduCounter->ComIPduCounterSize));
				
				if(PduR_ComTransmit(TransmisionReturnValue, IPdu->ComIPduHandleId, IPdu->ComIPduDataPtr) == E_NOT_OK)
				{
					(IPdu->ComIPduCounter)->ComCurrentCounterValue = (IPdu->ComIPduCounter->ComCurrentCounterValue - 1)%(power(IPdu->ComIPduCounter->ComIPduCounterSize));
				}
				IPdu->ComTxIPdu->ComNumberOfTransmissions--;
				if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 1)
				{
					delay(IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeRepetitionPeriod);
				}
				else
				{
					delay(IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeRepetitionPeriod);
				}
			}
			
			if((IPdu->ComTxIPdu->ComNumberOfTransmissions) == 0)
			{
				if(IPdu->ComTxIPdu->ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu->ComMinimumDelayTime);}
				else{}
			}
			else{}
	}
	else{}
}

void PeriodicModeSequence(ComIPdu_type *IPdu)
{
	if(CheckPeriodicTimeFired(IPdu) || IPdu->ComTxIPdu->ComFirstPeriodicModeEntry)
	{
		uint8 TransmisionReturnValue = E_OK;
		IPdu->ComTxIPdu->ComFirstPeriodicModeEntry = 0;
		ClearPeriodicTimeFired(IPdu);
		com_packSignalsToPdu(IPdu);
		if(IPdu->ComIPduCallout != NULL) {IPdu->ComIPduCallout(*(IPdu->ComPduIdRef), IPdu->PduInfo);}
		else{}
		Com_writeCounterValueToPduBuffer(IPdu, IPdu->ComIPduCounter->ComCurrentCounterValue);
		IPdu->ComIPduCounter->ComCurrentCounterValue = (IPdu->ComIPduCounter->ComCurrentCounterValue + 1)%(power(IPdu->ComIPduCounter->ComIPduCounterSize));
		
		if(PduR_ComTransmit(TransmisionReturnValue, IPdu->ComIPduHandleId, IPdu->ComIPduDataPtr) == E_NOT_OK)
		{
			IPdu->ComIPduCounter->ComCurrentCounterValue = (IPdu->ComIPduCounter->ComCurrentCounterValue - 1)%(power(IPdu->ComIPduCounter->ComIPduCounterSize));
		}
		if(IPdu->ComTxIPdu->ComMinimumDelayTime != 0){delay(IPdu->ComTxIPdu->ComMinimumDelayTime);}
		else{}
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
	uint8 currentIPduID ;
	for(currentIPduID = 0; currentIPduID < COM_NUM_OF_IPDU; currentIPduID++)
	{
		ComIPdu_type* IPdu = GET_IPDU(currentIPduID);
		if(IPdu !=NULL)
		{
			ComIPduGroup_type * IPduGroup =  IPdu->ComIPduGroupRef;
			if((IPduGroup == NULL || IPduGroup->IpduGroupFlag == STARTED) && IPdu->ComIPduDirection == SEND)
			{	
				/*-----------------------------------------------Notify RTE layer if IPDU is DEFERED--------------------------------------------------------*/
				if(IPdu->ComIPduSignalProcessing == DEFERRED && (IPdu->ComTxIPdu)->ComIsIPduDeferred == 1)
				{
					IPdu->ComTxIPdu->ComIsIPduDeferred = 0;
					if(IPdu->ComTxIPdu->ComTransmissionResult == E_OK)
					{
						uint16 signalID,signalGroupID;
						for(signalID=0; (IPdu->ComIPduSignalRef[signalID] != NULL); signalID++)
						{
							if(IPdu->ComIPduSignalRef[signalID]->ComNotification != NULL)
							{
								IPdu->ComIPduSignalRef[signalID]->ComNotification();
							}
							else{}
						}
						for(signalGroupID=0; (IPdu->ComIPduSignalGroupRef[signalGroupID] != NULL); signalGroupID++)
						{
							if(IPdu->ComIPduSignalGroupRef[signalGroupID]->ComNotification != NULL)
							{
								IPdu->ComIPduSignalGroupRef[signalGroupID]->ComNotification();
							}
							else{}
						}
					}
					else if(IPdu->ComTxIPdu->ComTransmissionResult == E_NOT_OK)
					{
						uint16 signalID, signalGroupID;
						for(signalID=0; (IPdu->ComIPduSignalRef[signalID] != NULL); signalID++)
						{
							if(IPdu->ComIPduSignalRef[signalID]->ComErrorNotification != NULL)
							{
								IPdu->ComIPduSignalRef[signalID]->ComErrorNotification();
							}
							else{}
						}
						for(signalGroupID=0; (IPdu->ComIPduSignalGroupRef[signalGroupID] != NULL); signalGroupID++)
						{
							if(IPdu->ComIPduSignalGroupRef[signalGroupID]->ComErrorNotification != NULL)
							{
								IPdu->ComIPduSignalGroupRef[signalGroupID]->ComErrorNotification();
							}
							else{}
						}
						
					}
					else{}
				
				}
				else{}
				
				/*---------------------------------------------------------Start sending IPDU-------------------------------------------------------------------*/
				if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 1)
				{
					switch(IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeMode)
					{
						case MIXED:
								DirectModeSequence(IPdu);
								
						case PERIODIC:
								PeriodicModeSequence(IPdu);
								break;
							
						case DIRECT:
								DirectModeSequence(IPdu);
					}
				}
				else if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 0)
				{
					switch(IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeMode)
					{
						case DIRECT:
								DirectModeSequence(IPdu);
								break;						
						case MIXED:
								DirectModeSequence(IPdu);
									
						case PERIODIC:
								PeriodicModeSequence(IPdu);
						
					}
				}
				else{}
			}
			else{}
			
		}
		else{}

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
  if (Signal.ComSignalUpdated)
	{
        /*[SWS_Com_00301] If ComIPduSignalProcessing for an I-PDU is configured to DEFERRED, 
        the AUTOSAR COM module shall first copy the I-PDU’s data within the
        Com_RxIndication function or the related TP reception functions respectively from 
        the PduR into COM. Then the AUTOSAR COM module shall invoke the configured
        ComNotifications for the included signals and signal groups asynchronously during
        the next call to Com_MainFunctionRx*/
				if (Signal.ComNotification != NULL)
				{
					Signal.ComNotification();
				}
				Signal.ComSignalUpdated = 0;
  }
	/*
	if (Signal.ComTimeout > 0)
	{
			timerDec(Signal.DeadlineMonitoringTimer);

			if(Signal.DeadlineMonitoringTimer == 0)
			{
					 switch(Signal.ComRxDataTimeoutAction )
					{ 
							 [SWS_Com_00470] If ComRxDataTimeoutAction is set to REPLACE, 
							the AUTOSAR COM module shall replace the signal’s value by
							its ComSignalInitValue when the reception deadline monitoring
							timer of a signal expires

							case TIMEOUT_REPLACE:
																		memcpy((uint8*)Signal.ComFGBuffer, (uint8*)Signal.ComSignalInitValue,(Signal.ComBitSize)/8);
																		break;

							[SWS_Com_00875] If ComRxDataTimeoutAction is set to SUBSTITUTE,
							the AUTOSAR COM module shall replace the signal's value by 
							its ComTimeoutSubstitutionValue when the reception deadline monitoring
							timer of a signal expires
							
							case SUBSTITUTE:
																		memcpy((uint8*)Signal.ComFGBuffer, (uint8*)Signal.ComTimeoutSubstitutionValue,(Signal.ComBitSize)/8);
																		break;

					}

					if (Signal.ComTimeoutNotification != NULL)
					{
							Signal.ComTimeoutNotification();
					}			 
					else{}
					
					// Restart the timer
					Signal.DeadlineMonitoringTimer = Signal.ComTimeout;
			}
			else{}

	}
	else{}*/
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
	  ComGroupSignal_type* GroupSignal;
		boolean SGupdated = 0;
		for(ComMainRxGroupSignalId=0 ; SignalGroup.ComGroupSignal[ComMainRxGroupSignalId] != NULL; ComMainRxGroupSignalId++)
    {
			GroupSignal = SignalGroup.ComGroupSignal[ComMainRxGroupSignalId];
			
			if (GroupSignal->ComSignalUpdated)
			{
						/*[SWS_Com_00301] If ComIPduSignalProcessing for an I-PDU is configured to DEFERRED, 
						the AUTOSAR COM module shall first copy the I-PDU’s data within the
						Com_RxIndication function or the related TP reception functions respectively from 
						the PduR into COM. Then the AUTOSAR COM module shall invoke the configured
						ComNotifications for the included signals and signal groups asynchronously during
						the next call to Com_MainFunctionRx*/
						SGupdated = 1;
						GroupSignal->ComSignalUpdated = 0;
			}
		}
		if (SGupdated && SignalGroup.ComNotification != NULL)
		{
			SignalGroup.ComNotification();
		}
		
    /*
    if (SignalGroup.ComTimeout > 0)
    {
        timerDec(SignalGroup.DeadlineMonitoringTimer);

        if(SignalGroup.DeadlineMonitoringTimer == 0)
        {
            for(ComMainRxGroupSignalId=0 ;(SignalGroup.ComGroupSignal != NULL && SignalGroup.ComGroupSignal[ComMainRxGroupSignalId] != NULL) ; ComMainRxGroupSignalId++)
            {
                GroupSignal = SignalGroup.ComGroupSignal[ComMainRxGroupSignalId];

                switch(GroupSignal->ComRxDataTimeoutAction )
                { 
	                  case TIMEOUT_REPLACE:
														memcpy((uint8*)SignalGroup.ComShadowBuffer, (uint8*)GroupSignal->ComSignalInitValue,(GroupSignal->ComBitSize)/8);
														break;
                    case SUBSTITUTE:
														memcpy((uint8*)SignalGroup.ComShadowBuffer, (uint8*)GroupSignal->ComTimeoutSubstitutionValue,(GroupSignal->ComBitSize)/8);
														//Com_WriteSignalDataToPdu(GroupSignal->ComHandleId, GroupSignal->ComTimeoutSubstitutionValue);
														break;
										
                }

            }

            if (SignalGroup.ComTimeoutNotification != NULL)
            {
								SignalGroup.ComTimeoutNotification();
						}
            else{}

            // Restart the timer
            SignalGroup.DeadlineMonitoringTimer = SignalGroup.ComTimeout;
        }

    }
    else{}*/

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
        
	const ComSignal_type *Signal = NULL;
  const ComSignalGroup_type *SignalGroup =NULL;
	uint8 ComMainRxSignalId;
  uint8 ComMainRxSignalGroupId;

	if (IPdu.ComIPduSignalProcessing == DEFERRED && IPdu.ComIPduDirection == RECEIVE)
	{
			//For each signal at PDU
			for ( ComMainRxSignalId = 0; (IPdu.ComIPduSignalRef[ComMainRxSignalId] != NULL); ComMainRxSignalId++)
			{
					//Get signal
					Signal = IPdu.ComIPduSignalRef[ComMainRxSignalId];
	
					Com_MainFunctionRxSignal(*Signal);
					CopySignalfromBGtoFG(ComMainRxSignalId);	 
			}

			for (ComMainRxSignalGroupId = 0; IPdu.ComIPduSignalGroupRef[ComMainRxSignalGroupId] != NULL; ComMainRxSignalGroupId++)
			{
					SignalGroup = IPdu.ComIPduSignalGroupRef[ComMainRxSignalGroupId];

					Com_MainFunctionRxSignalGroup(*SignalGroup);

					CopySignalGroupfromBGtoSB(ComMainRxSignalGroupId);

			}         

	} 
	else{}
}
