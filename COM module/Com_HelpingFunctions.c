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
 boolean is_com_initiated(const Com_ConfigType* config)
 {
    return config->com_initiated;
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
		
	}
	
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
			if(signal->ComTransferProperty == TRIGGERED)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
		}
		else if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 0 && (IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT || IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeNumberOfRepetitions;
			if(signal->ComTransferProperty == TRIGGERED)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
		}
		returnValue = E_OK;
	}
	return returnValue;
}


uint8 com_pdu_transmissions_handle_signalGroup(ComIPdu_type* IPdu, ComSignalGroup_type* signalGroup)
{
	uint8 returnValue;
	if(IPdu == NULL || signalGroup == NULL)
	{
		returnValue = E_NOT_OK;
	}
	else
	{
		if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 1 && (IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == DIRECT || IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeNumberOfRepetitions;
			if(signalGroup->ComTransferProperty == TRIGGERED)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
		}
		else if(IPdu->ComTxIPdu.ComCurrentTransmissionSelection == 0 && (IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT || IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeNumberOfRepetitions;
			if(signalGroup->ComTransferProperty == TRIGGERED)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu.ComNumberOfTransmissions += 1;
			}
		}
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
		Signal = IPdu->ComIPduSignalRef[ComSignalId];
		if(signal->ComSignalFilterResult == 1)
		{
			TMS = 1;
			break;
		}
	}
	if(TMS == 0)
	{
		for (uint32 ComSignalGroupId = 0; (IPdu->ComIPduSignalRef[ComSignalGroupId] != NULL); ComSignalGroupId++)
		{
			//Get signal
			signalGroup = IPdu->ComIPduSignalGroupRef[ComSignalGroupId];
			if(signalGroup->ComSignalGroupFilterResult == 1)
			{
				TMS = 1;
				break;
			}
		}
	}
	
	return TMS;
}
