/*********************************************************************************
 *                                    Includes                                   *
 ********************************************************************************/


#include "include/Com.h"
#include "include/Com_Types.h"
#include "include/Com_Cfg.h"
#include "include/ComMacros.h"
#include <cstddef>

/**********************************************************************************
 *                             Functions Definitions                              *
 **********************************************************************************/
 

boolean check_Data_Sequence(ComIPdu_type *Ipdu,ComIPdu_type *Ipdu_Rx)
{
	uint8 excounter,recounter;
	void * data=NULL;
	Ipdu->ComIPduCounter->ComIPduCounterSize
    Ipdu->ComIPduCounter->ComIPduCounterStartPosition

    unit64 mask = 0;
	for(uint8 i = 0; i<Ipdu->ComIPduCounter->ComIPduCounterSize; i++)
	{
		uint64 currentmask = 1u<< (ComIPduCounter->ComIPduCounterStartPosition+i);
		mask = mask | currentmask;
	}
    memcpy((uint8 *)data,(uint8 *)Ipdu->ComIPduDataPtr,Ipdu->ComIPduLength)
	mask = data & mask
}

boolean Com_ProcessTxSignalFilter(ComSignal_type* signalStruct, uint64 oldData, uint64 newData)
{
	boolean filterResult = 0;
	ComSignalType_type signalType = signalStruct->ComSignalType;
	uint64 mask = (signalStruct->comFilter)->ComFilterMask;
	if(signalType == SINT8 || signalType == SINT16 || signalType == SINT32 || signalType == SINT64)
	{
		sint64 signedNewData = (sint64)newData;
		sint64 signedOldData = (sint64)oldData;
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (mask & signedNewData) != (mask & signedOldData) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_X) && ( (mask & signedNewData) != (signalStruct->comFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ( (mask & signedNewData) == (signalStruct->comFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(signedNewData <= ComFilterMax && signedNewData >= ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(signedNewData > ComFilterMax || signedNewData < ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == ONE_EVERY_N) && ((signalStruct->comFilter)->ComFilterOccurrence == (signalStruct->comFilter)->ComFilterOffset) )
		{
			filterResult = 1;
			(signalStruct->comFilter)->ComFilterOccurrence += 1;
			
			if((signalStruct->comFilter)->ComFilterOccurrence == (signalStruct->comFilter)->ComFilterPeriod)
			{
				(signalStruct->comFilter)->ComFilterOccurrence = 0;
			}
			
		}
		else{}
	}
	else
	{
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (mask & newData) != (mask & oldData) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_X) && ( (mask & newData) != (signalStruct->comFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ( (mask & newData) == (signalStruct->comFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(newData <= ComFilterMax && newData >= ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(newData > ComFilterMax || newData < ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == ONE_EVERY_N) && ((signalStruct->comFilter)->ComFilterOccurrence == (signalStruct->comFilter)->ComFilterOffset) )
		{
			filterResult = 1;
			(signalStruct->comFilter)->ComFilterOccurrence += 1;
			
			if((signalStruct->comFilter)->ComFilterOccurrence == (signalStruct->comFilter)->ComFilterPeriod)
			{
				(signalStruct->comFilter)->ComFilterOccurrence = 0;
			}
			
		}
		else{}
	}
	
	return filterResult;
}

boolean Com_ProcessTxSignalFilter_float(ComSignal_type* signalStruct, float64 oldData, float64 newData)
{
	boolean filterResult = 0;
	ComSignalType_type signalType = signalStruct->ComSignalType;
	if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
	{
		filterResult = 1;
	}
	else if( ((signalStruct->comFilter)->ComFilterAlgorithm == ONE_EVERY_N) && ((signalStruct->comFilter)->ComFilterOccurrence == (signalStruct->comFilter)->ComFilterOffset) )
	{
		filterResult = 1;
		(signalStruct->comFilter)->ComFilterOccurrence += 1;
		
		if((signalStruct->comFilter)->ComFilterOccurrence == (signalStruct->comFilter)->ComFilterPeriod)
		{
			(signalStruct->comFilter)->ComFilterOccurrence = 0;
		}
		else{}
	}
	else{}
	
	return filterResult;
}




uint8 com_pdu_transmissions_handle_signal(ComIPdu_type* IPdu, ComSignal_type* signal)
{
	uint8 returnValue;
	if(IPdu == NULL || signal == NULL)
	{
		returnValue = E_NOT_OK;
	}
	else
	{
		if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 1 && (IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == DIRECT || IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeNumberOfRepetitions;
			if(signal->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE && signal->ComIsSignalChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else{}
		}
		else if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 0 && (IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT || IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeNumberOfRepetitions;
			if(signal->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE && signal->ComIsSignalChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else{}
		}
		else{}
		returnValue = E_OK;
	}
	return returnValue;
}


uint8 com_pdu_transmissions_handle_signalGroup(ComIPdu_type* IPdu, ComSignalGroup_type* signalGroup)
{
	uint8 returnValue;
	if(IPdu == NULL || signalGroup == NULL || (IPdu->ComIPduGroupRef != NULL && (IPdu->ComIPduGroupRef)->IpduGroupFlag == STOPPED) )
	{
		returnValue = E_NOT_OK;
	}
	else
	{
		if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 1 && (IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == DIRECT || IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeNumberOfRepetitions;
			if(signalGroup->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE && signalGroup->ComIsSignalGroupChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else{}
		}
		else if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 0 && (IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT || IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeNumberOfRepetitions;
			if(signalGroup->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += (N+1);
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE && signalGroup->ComIsSignalGroupChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += (N+1);
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else{}
		}
		else{}
		returnValue = E_OK;
	}
	return returnValue;
}


boolean com_pdu_transmissionsModeSelection(ComIPdu_type* IPdu)
{
	boolean TMS = 0;
	for (uint32 ComSignalId = 0; (IPdu->ComIPduSignalRef[ComSignalId] != NULL); ComSignalId++)
	{
		//Get signal
		ComSignal_type* signal = IPdu->ComIPduSignalRef[ComSignalId];
		if(signal->ComSignalFilterResult == 1)
		{
			TMS = 1;
			break;
		}
		else{}
	}
	if(TMS == 0)
	{
		for (uint32 ComSignalGroupId = 0; (IPdu->ComIPduSignalRef[ComSignalGroupId] != NULL); ComSignalGroupId++)
		{
			//Get signal
			ComSignalGroup_type* signalGroup = IPdu->ComIPduSignalGroupRef[ComSignalGroupId];
			if(signalGroup->ComSignalGroupFilterResult == 1)
			{
				TMS = 1;
				break;
			}
			else{}
		}
	}
	/*--------------------------------------------------------------------------------------------------------------------------------------------------------
	When a call to Com_SendSignal or Com_SendSignalGroup results into a change of the transmission mode of a started I-PDU to the transmission mode PERIODIC
	or MIXED, then the AUTOSAR COM module shall start the new transmission cycle with a call to PduR_ComTransmit within the next main function at the latest.
	--------------------------------------------------------------------------------------------------------------------------------------------------------*/
	boolean oldTMS = IPdu->ComTxIPdu.ComCurrentTransmissionSelection;
	if((oldTMS == 0 && TMS == 1 && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode != DIRECT)
		||(oldTMS == 1 && TMS == 0 && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == DIRECT && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode != DIRECT))
	{
		 IPdu->ComTxIPdu.ComFirstPeriodicModeEntry = 1;
	}
	else if((oldTMS == 0 && Mode == 1 && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode != DIRECT && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == DIRECT)
			||(oldTMS == 1 && Mode == 0 && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode != DIRECT && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT))
	{
		IPdu->ComTxIPdu.ComFirstDirectModeEntry = 1;
	}
	else{}
	
	return TMS;
}

com_packSignalsToPdu(ComIPdu_type* IPdu)
{
	for(uint16 ComSignalId = 0; (IPdu->ComIPduSignalRef[ComSignalId] != NULL); ComSignalId++)
	{
		ComSignal_type* signal = IPdu->ComIPduSignalRef[ComSignalId];
		Com_WriteSignalDataToPdu(signal->ComHandleId, signal->ComFGBuffer);
	}
	void * dataAddress = NULL;
	for(uint16 ComSignaGrouplId = 0; (IPdu->ComIPduSignalGroupRef[ComSignaGrouplId] != NULL); ComSignaGrouplId++)
	{
		ComSignalGroup_type* signalGroup = IPdu->ComIPduSignalGroupRef[ComSignaGrouplId];
		for(uint16 ComGroupSignalId = 0; (signalGroup->ComIPduGroupSignalRef[ComGroupSignalId] != NULL); ComGroupSignalId++)
		{
			ComGroupSignal_type* groupSignal = signalGroup->ComIPduGroupSignalRef[ComGroupSignalId];
			CopyGroupSignalFromFGtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, dataAddress);
			Com_WriteGroupSignalDataToPdu(groupSignal->ComHandleId, dataAddress);
		}
		
	}
}
