/*********************************************************************************
 *                                    Includes                                   *
 ********************************************************************************/

#include "stdio.h"
#include "../include/Com.h"
#include "../include/Com_Types.h"
#include "../include/Com_Cfg.h"
#include "../include/ComMacros.h"
#include "../include/com_buffers.h"
#include "../include/PeriodicMode_HelpingFunctions.h"

//most updated version

/**********************************************************************************
 *                             Functions Definitions                              *
 **********************************************************************************/
 
void delay(uint32 seconds)
{
	uint64 volatile time;
	uint8 i;
	for(i=0; i<10; i++)
	{
		while(seconds>0){
			time = 727240;  // 0.1sec at 80 MHz
			while(time){
				time--;
			}
			seconds--;
		}
	}
}

uint8 power(unsigned n) 
{ 
    // Initialize result to 1 
    uint8 pow = 1; 
		uint8 i;
    // Multiply x for n times 
    for (i = 0; i < n; i++) { 
        pow = pow * 2; 
    } 
    
    return pow; 
} 

boolean Com_ProcessTxSignalFilter(ComSignal_type* signalStruct, uint64 oldData, uint64 newData)
{
	boolean filterResult = 0;
	ComSignalType_type signalType = signalStruct->ComSignalType;
	sint64 mask = (signalStruct->ComFilter)->ComFilterMask;
	sint64 ComFilterMax = signalStruct->ComFilter->ComFilterMax;
	sint64 ComFilterMin = signalStruct->ComFilter->ComFilterMin;
	if(signalType == SINT8 || signalType == SINT16 || signalType == SINT32 || signalType == SINT64)
	{
		sint64 signedNewData = (sint64)newData;
		sint64 signedOldData = (sint64)oldData;
		if((signalStruct->ComFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (mask & signedNewData) != (mask & signedOldData) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_X) && ( (mask & signedNewData) != (signalStruct->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->ComFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ( (mask & signedNewData) == (signalStruct->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( (signalStruct->ComFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(signedNewData <= ComFilterMax && signedNewData >= ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( (signalStruct->ComFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(signedNewData > ComFilterMax || signedNewData < ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if(signalStruct->ComFilter->ComFilterAlgorithm == ONE_EVERY_N)
		{
		    if((signalStruct->ComFilter->ComFilterOccurrence % signalStruct->ComFilter->ComFilterPeriod)== signalStruct->ComFilter->ComFilterOffset)
		    {
		        filterResult = 1;
		    }
			(signalStruct->ComFilter)->ComFilterOccurrence += 1;
		}
		else{}
	}
	else
	{
		if((signalStruct->ComFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((signalStruct->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (mask & newData) != (mask & oldData) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_X) && ( (mask & newData) != (signalStruct->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((signalStruct->ComFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ( (mask & newData) == (signalStruct->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( (signalStruct->ComFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(newData <= ComFilterMax && newData >= ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( (signalStruct->ComFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(newData > ComFilterMax || newData < ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if(signalStruct->ComFilter->ComFilterAlgorithm == ONE_EVERY_N)
		{
		    if((signalStruct->ComFilter->ComFilterOccurrence % signalStruct->ComFilter->ComFilterPeriod)== signalStruct->ComFilter->ComFilterOffset)
		    {
		        filterResult = 1;
		    }
			(signalStruct->ComFilter)->ComFilterOccurrence += 1;
		}
		else{}
	}
	
	return filterResult;
}

boolean Com_ProcessTxSignalFilter_float(ComSignal_type* signalStruct, float64 oldData, float64 newData)
{
	boolean filterResult = 0;
	ComSignalType_type signalType = signalStruct->ComSignalType;
	if((signalStruct->ComFilter)->ComFilterAlgorithm == ALWAYS)
	{
		filterResult = 1;
	}
	else if(signalStruct->ComFilter->ComFilterAlgorithm == ONE_EVERY_N)
	{
		   if((signalStruct->ComFilter->ComFilterOccurrence % signalStruct->ComFilter->ComFilterPeriod)== signalStruct->ComFilter->ComFilterOffset)
		   {
		        filterResult = 1;
		   }
			(signalStruct->ComFilter)->ComFilterOccurrence += 1;
	}
	else{}
	
	return filterResult;
}


boolean Com_ProcessTxSignalFilter_groupsignal(ComGroupSignal_type* signalStruct, uint64 oldData, uint64 newData)
{
	boolean filterResult = 0;
	ComSignalGroup_type *SignalGroup = GET_SIGNALGROUP(signalStruct->SignalGroupId);
	ComSignalType_type signalType = signalStruct->ComSignalType;
	sint64 mask = (SignalGroup->ComFilter)->ComFilterMask;
	sint64 ComFilterMax = SignalGroup->ComFilter->ComFilterMax;
	sint64 ComFilterMin = SignalGroup->ComFilter->ComFilterMin;
	if(signalType == SINT8 || signalType == SINT16 || signalType == SINT32 || signalType == SINT64)
	{
		sint64 signedNewData = (sint64)newData;
		sint64 signedOldData = (sint64)oldData;
		if((SignalGroup->ComFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((SignalGroup->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (mask & signedNewData) != (mask & signedOldData) )
		{
			filterResult = 1;
		}
		else if( ((SignalGroup->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_X) && ( (mask & signedNewData) != (SignalGroup->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((SignalGroup->ComFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ( (mask & signedNewData) == (SignalGroup->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( (SignalGroup->ComFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(signedNewData <= ComFilterMax && signedNewData >= ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( (SignalGroup->ComFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(signedNewData > ComFilterMax || signedNewData < ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if(SignalGroup->ComFilter->ComFilterAlgorithm == ONE_EVERY_N)
		{
		    if((SignalGroup->ComFilter->ComFilterOccurrence % SignalGroup->ComFilter->ComFilterPeriod)== SignalGroup->ComFilter->ComFilterOffset)
		    {
		        filterResult = 1;
		    }
			(SignalGroup->ComFilter)->ComFilterOccurrence += 1;
		}
		else{}
	}
	else
	{
		if((SignalGroup->ComFilter)->ComFilterAlgorithm == ALWAYS)
		{
			filterResult = 1;
		}
		else if( ((SignalGroup->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (mask & newData) != (mask & oldData) )
		{
			filterResult = 1;
		}
		else if( ((SignalGroup->ComFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_X) && ( (mask & newData) != (SignalGroup->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( ((SignalGroup->ComFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ( (mask & newData) == (SignalGroup->ComFilter)->ComFilterX) )
		{
			filterResult = 1;
		}
		else if( (SignalGroup->ComFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(newData <= ComFilterMax && newData >= ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if( (SignalGroup->ComFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(newData > ComFilterMax || newData < ComFilterMin)
			{
				filterResult = 1;
			}
		}
		else if(SignalGroup->ComFilter->ComFilterAlgorithm == ONE_EVERY_N)
		{
		    if((SignalGroup->ComFilter->ComFilterOccurrence % SignalGroup->ComFilter->ComFilterPeriod)== SignalGroup->ComFilter->ComFilterOffset)
		    {
		        filterResult = 1;
		    }
			(SignalGroup->ComFilter)->ComFilterOccurrence += 1;
		}
		else{}
	}
	
	return filterResult;
}

boolean Com_ProcessTxSignalFilter_float_groupsignal(ComGroupSignal_type* signalStruct, float64 oldData, float64 newData)
{
	boolean filterResult = 0;
	ComSignalGroup_type *SignalGroup = GET_SIGNALGROUP(signalStruct->SignalGroupId);
	ComSignalType_type signalType = signalStruct->ComSignalType;
	if((SignalGroup->ComFilter)->ComFilterAlgorithm == ALWAYS)
	{
		filterResult = 1;
	}
	else if(SignalGroup->ComFilter->ComFilterAlgorithm == ONE_EVERY_N)
		{
		    if((SignalGroup->ComFilter->ComFilterOccurrence % SignalGroup->ComFilter->ComFilterPeriod)== SignalGroup->ComFilter->ComFilterOffset)
		    {
		        filterResult = 1;
		    }
			(SignalGroup->ComFilter)->ComFilterOccurrence += 1;
		}
	else{}
	
	return filterResult;
}




uint8 com_pdu_transmissions_handle_signal(ComIPdu_type* IPdu, ComSignal_type* signal)
{
	uint8 returnValue;
	if(IPdu == NULL || signal == NULL || (IPdu->ComIPduGroupRef != NULL && (IPdu->ComIPduGroupRef)->IpduGroupFlag == STOPPED) )
	{
		returnValue = E_NOT_OK;
	}
	else
	{
		if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 1 && (IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeMode == DIRECT || IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeNumberOfRepetitions;
			if(signal->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE && signal->ComIsSignalChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += N;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
			}
			else{}
		}
		else if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 0 && (IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeMode == DIRECT || IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeNumberOfRepetitions;
			if(signal->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += (N+1);
			}
			else if(signal->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE && signal->ComIsSignalChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += (N+1);
			}
			else if(signal->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signal->ComIsSignalChanged == 1)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
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
		if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 1 && (IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeMode == DIRECT || IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeNumberOfRepetitions;
			if(signalGroup->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE && signalGroup->ComIsSignalGroupChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += N;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
			}
			else{}
		}
		else if(IPdu->ComTxIPdu->ComCurrentTransmissionSelection == 0 && (IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeMode == DIRECT || IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeMode == MIXED))
		{
			uint8 N = IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeNumberOfRepetitions;
			if(signalGroup->ComTransferProperty == TRIGGERED && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += (N+1);
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_WITHOUT_REPETITION)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE && signalGroup->ComIsSignalGroupChanged == 1 && N>0)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += (N+1);
			}
			else if(signalGroup->ComTransferProperty == TRIGGERED_ON_CHANGE_WITHOUT_REPETITION && signalGroup->ComIsSignalGroupChanged == 1)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions += 1;
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
	uint32 ComSignalId;
	boolean oldTMS = IPdu->ComTxIPdu->ComCurrentTransmissionSelection;
	ComTxModeMode_type oldMode;
	ComTxModeMode_type newMode;
	for (ComSignalId = 0; (IPdu->ComIPduSignalRef[ComSignalId] != NULL); ComSignalId++)
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
		uint32 ComSignalGroupId;
		for (ComSignalGroupId = 0; (IPdu->ComIPduSignalGroupRef[ComSignalGroupId] != NULL); ComSignalGroupId++)
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
	
	if(oldTMS!=TMS)
	{
		if(oldTMS == 1)
		{
			oldMode = IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeMode;
			newMode = IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeMode;
		}
		else
		{
			newMode = IPdu->ComTxIPdu->ComTxModeTrue->ComTxMode->ComTxModeMode;
			oldMode = IPdu->ComTxIPdu->ComTxModeFalse->ComTxMode->ComTxModeMode;
		}
		
		if(oldMode!=newMode)
		{
			if(oldMode==DIRECT && newMode!=DIRECT)
			{
				Std_ReturnType returnValue = Com_InitPeriodicModeForIPdu(IPdu);
				if(newMode == MIXED){IPdu->ComTxIPdu->ComNumberOfTransmissions +=1;}
				else{IPdu->ComTxIPdu->ComFirstPeriodicModeEntry = 1;}
			}
			else if(oldMode!=DIRECT && newMode==DIRECT)
			{
				IPdu->ComTxIPdu->ComNumberOfTransmissions +=1;
				DisableIPduTimer(IPdu);
			}
			else{}
		}
		else{}
	}
	else{}
	
	return TMS;
}

void com_packSignalsToPdu(ComIPdu_type* IPdu)
{
	uint16 ComSignalId;
	uint64 data;
	void * dataAddress = &data;
	uint16 ComSignaGrouplId;
	for(ComSignalId = 0; (IPdu->ComIPduSignalRef[ComSignalId] != NULL); ComSignalId++)
	{
		ComSignal_type* signal = IPdu->ComIPduSignalRef[ComSignalId];
		Com_WriteSignalDataToPdu(signal->ComHandleId, signal->ComFGBuffer);
	}
	
	for(ComSignaGrouplId = 0; (IPdu->ComIPduSignalGroupRef[ComSignaGrouplId] != NULL); ComSignaGrouplId++)
	{
		ComSignalGroup_type* signalGroup = IPdu->ComIPduSignalGroupRef[ComSignaGrouplId];
		uint16 ComGroupSignalId;
		for(ComGroupSignalId = 0; (signalGroup->ComGroupSignal[ComGroupSignalId] != NULL); ComGroupSignalId++)
		{
			const ComGroupSignal_type* groupSignal = signalGroup->ComGroupSignal[ComGroupSignalId];
			CopyGroupSignalFromFGtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, dataAddress);
			Com_WriteGroupSignalDataToPdu(groupSignal->ComGlobalGroupSignalID, dataAddress);
		}
		
	}
}



uint8 Com_writeCounterValueToPduBuffer(ComIPdu_type *IPdu, uint8 counterdata)
{
	uint8 returnValue = E_OK;
	// Get PDU
	
	if(IPdu != NULL)
	{
		const ComIPduCounter_type* PduCounter = IPdu->ComIPduCounter;
		uint8 mask;
		uint32 BitPosition;
		uint8 pduStartByte;
		uint8 BitOffsetInByte;
		uint8* pduBufferBytesptr = NULL;
		uint8 counterLength;
		if(PduCounter != NULL)
		{
			BitPosition = PduCounter->ComIPduCounterStartPosition;
			pduStartByte = BitPosition / 8;
			BitOffsetInByte = BitPosition % 8;
			pduBufferBytesptr = (uint8*)(IPdu->ComIPduDataPtr);
			counterLength = PduCounter->ComIPduCounterSize;

			pduBufferBytesptr += pduStartByte;

			// clear unused bits in counter data
			mask = 255;
			mask = mask >> (8 - counterLength);
			counterdata = counterdata & mask;


			if(8-BitOffsetInByte >= counterLength)
			{
				// if whole counter is contained in one byte
				uint8 i;
				mask = 0;
				for(i = 0; i<counterLength; i++)
				{
					mask = mask | (1u << (BitOffsetInByte+i) );
				}
				mask = ~mask;
				*pduBufferBytesptr = (*pduBufferBytesptr) & mask;

				counterdata = counterdata << BitOffsetInByte;
				*pduBufferBytesptr = (*pduBufferBytesptr) | counterdata;
			}
			else
			{
				// if counter is divided on two bytes
				// for first byte
				uint8 bitsNumberInSecondByte;
				uint8 i;
				mask = 255;
				mask = ~(mask << BitOffsetInByte);
				*pduBufferBytesptr = (*pduBufferBytesptr) & mask;

				mask = 255;
				mask = mask >> BitOffsetInByte;
				mask = mask & counterdata;
				mask = mask << BitOffsetInByte;
				*pduBufferBytesptr = (*pduBufferBytesptr) | mask;

				//for second byte
				pduBufferBytesptr += 1;
				bitsNumberInSecondByte = counterLength - (8 - BitOffsetInByte);
				mask = 255;
				mask = ~( mask >> (8 - bitsNumberInSecondByte) );
				*pduBufferBytesptr = (*pduBufferBytesptr) & mask;

				mask = 0;
				for(i = 0; i<bitsNumberInSecondByte; i++)
				{
					mask = mask | (1u << ((8 - BitOffsetInByte) + i));
				}
				mask = mask & counterdata;
				mask = mask >> (8 - BitOffsetInByte);
				*pduBufferBytesptr = (*pduBufferBytesptr) | mask;
			}
		}
		else
		{
			returnValue = E_NOT_OK;
		}
	}
	else
	{
		returnValue = E_NOT_OK;
	}
	
	return returnValue;
}


uint8 check_Data_Sequence(ComIPdu_type *Ipdu)
{
	// Get PDU
	
	const ComIPduCounter_type* PduCounter1 = Ipdu->ComIPduCounter;
	
	uint8 counterValue1 = 0;
	uint8 mask1;
	uint8 i;
	uint32 BitPosition1 = PduCounter1->ComIPduCounterStartPosition;

	uint32 pduStartByte1 = BitPosition1 / 8;
	
	uint32 BitOffsetInByte1 = BitPosition1 % 8;
	
	uint8 *pduBufferBytesptr1 = (uint8 *)(Ipdu->ComIPduDataPtr);
	
	uint8 counterLength1 = PduCounter1->ComIPduCounterSize;
   
	pduBufferBytesptr1 += pduStartByte1;
  
	if(8-BitOffsetInByte1 >= counterLength1)
	{
		// if whole counter is contained in one byte
		mask1 = 0;
		for(i = 0; i<counterLength1; i++)
		{
			mask1 = mask1 | (1u << (BitOffsetInByte1+i) );
		}
		mask1 = (*pduBufferBytesptr1) & mask1;
		mask1 = mask1 >> BitOffsetInByte1;
		counterValue1 = mask1;
	}
	else
	{
		// if counter is divided on two bytes
		// for first byte
		uint32 bitsNumberInSecondByte1;
		mask1 = 255;
		mask1 = mask1 << BitOffsetInByte1;
		mask1 = (*pduBufferBytesptr1) & mask1;
		mask1 = mask1 >> BitOffsetInByte1;
		counterValue1 = mask1;

		//for second byte
		pduBufferBytesptr1 += 1;
		bitsNumberInSecondByte1 = counterLength1 - (8 - BitOffsetInByte1);
		mask1 = 255;
		mask1 = mask1 >> (8 - bitsNumberInSecondByte1);
		mask1 = (*pduBufferBytesptr1) & mask1;
		mask1 = mask1 << (8-BitOffsetInByte1);
		counterValue1 = mask1 | counterValue1;
	}

	return counterValue1;
}
