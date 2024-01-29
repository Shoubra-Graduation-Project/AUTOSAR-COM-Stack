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
uint8 Com_ProcessTxSignalFilter(ComSignal_type* signalStruct, uint64 oldData, uint64 newData)
{
	uint8 filterResult = 0;
	ComSignalType_type signalType = signalStruct->ComSignalType;
	if(signalType == SINT8 || signalType == SINT16 || signalType == SINT32 || signalType == SINT64)
	{
		sint64 signedNewData = (sint64)newData;
		sint64 signedOldData = (sint64)oldData;
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && signedNewData != signedOldData)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (signedNewData != (signalStruct->comFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (signedNewData == (signalStruct->comFilter)->ComFilterX) )
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
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == ONE_EVERY_N) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			filterResult = 1;
			if((signalStruct->comFilter)->ComFilterOffset < (signalStruct->comFilter)->ComFilterPeriod-1)
			{
				(signalStruct->comFilter)->ComFilterOffset += 1;
			}
			else
			{
				(signalStruct->comFilter)->ComFilterOffset = 0;
			}
		}
	}
	else
	{
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && newData != oldData)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (newData != (signalStruct->comFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (newData == (signalStruct->comFilter)->ComFilterX) )
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
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == ONE_EVERY_N) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			filterResult = 1;
			if((signalStruct->comFilter)->ComFilterOffset < (signalStruct->comFilter)->ComFilterPeriod-1)
			{
				(signalStruct->comFilter)->ComFilterOffset += 1;
			}
			else
			{
				(signalStruct->comFilter)->ComFilterOffset = 0;
			}
		}
	}
	
	return filterResult;
}

uint8 Com_ProcessTxSignalFilter_float(ComSignal_type* signalStruct, float64 oldData, float64 newData)
{
	uint8 filterResult = 0;
	ComSignalType_type signalType = signalStruct->ComSignalType;
	if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && newData != oldData)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (newData != (signalStruct->comFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (newData == (signalStruct->comFilter)->ComFilterX) )
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
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == ONE_EVERY_N) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			filterResult = 1;
			if((signalStruct->comFilter)->ComFilterOffset < (signalStruct->comFilter)->ComFilterPeriod-1)
			{
				(signalStruct->comFilter)->ComFilterOffset += 1;
			}
			else
			{
				(signalStruct->comFilter)->ComFilterOffset = 0;
			}
		}
	}
	
	return filterResult;
}	
