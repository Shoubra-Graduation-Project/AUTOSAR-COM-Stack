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
