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


/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_Init    
 *    
 *    Service Id: 0x01                                                             *
 * 
 *    Parameters (in): config->  Pointer to the COM module's configuration data.
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  This service initializes internal and external interfaces and 
 *                  variables of the COM module layer for the further
 *                  processing. After calling this function the inter-ECU 
 *                  communication is still disabled.  
 * 
 *********************************************************************************/
const Com_ConfigType * ComConfig;

void Com_Init (const Com_ConfigType* config)
{

        // Initialize global and static variables
        ComConfig = config;

	    ComIPdu_type *IPdu;
    	ComSignal_type *Signal;
		ComSignalGroup_type *SignalGroup;
	    ComGroupSignal_type *GroupSignal;

	    uint8 ComInitPduId;
		uint8 ComInitSignalId;
        uint8 ComInitGroupSignalId;
        uint8 ComInitSignalGroupId;

	    // Loop over all I-PDUs
        for (ComInitPduId = 0; ComInitPduId < COM_NUM_OF_IPDU ; ComInitPduId++)
		 {
            
			// Get IPdu
		    IPdu = GET_IPdu(ComInitPduId);
	
           // Fill not used areas within an I-PDU with a value determined by configuration parameter ComTxIPduUnusedAreasDefault
	       if (IPdu->ComIPduDirection == SEND)
		    {
			   memset((void *)IPdu->ComIPduDataPtr, IPdu->ComTxIPdu.ComTxIPduUnusedAreasDefault, IPdu->ComIPduLength);
	    	}
			else
			{

			}
            // For each signal at the I-PDU
			for ( ComInitSignalId = 0; (IPdu->ComIPduSignalRef[ComInitSignalId] != NULL); ComInitSignalId++)
			{
			   // Get Signal
               Signal = IPdu->ComIPduSignalRef[ComInitSignalId];

               /*
                 initialize each signal of n-bit sized signal type on sender and receiver side
                 with the lower n-bits of its configuration parameter ComSignalInitValue
               */
            /*  memcpy(Signal->ComSignalDataPtr, config->ComSignal[ComInitSignalId].ComSignalInitValue, Signal->ComBitSize/8);*/

               uint8 *dest = (uint8 *) Signal->ComSignalDataPtr;
               uint8 *src =  (uint8 *) config->ComSignal[ComInitSignalId].ComSignalInitValue;

                    for(int i= 0; i < Signal->ComBitSize/8 ; i++)
                   {
                       *dest = src++;
                        dest++;
                   }
               // Delete update bit for all signals
			   CLEARBIT(IPdu->ComIPduDataPtr, Signal->ComUpdateBitPosition);


			}
            // For each signal group at the I-PDU
		    for (ComInitSignalGroupId = 0; IPdu[ComInitSignalGroupId].ComIPduSignalGroupRef[ComInitSignalGroupId] != NULL; ComInitSignalGroupId++)
            {
                /* Get SignalGroup */
                SignalGroup = IPdu[ComInitSignalGroupId].ComIPduSignalGroupRef[ComInitSignalGroupId];
    
                // Delete update bit for all signal groups
                CLEARBIT(IPdu->ComIPduDataPtr, SignalGroup->ComUpdateBitPosition);

            

                // For each group signal at signal group
                for(ComInitGroupSignalId=0 ;(SignalGroup->ComGroupSignal != NULL && SignalGroup->ComGroupSignal[ComInitGroupSignalId] != NULL) ; ComInitGroupSignalId++)
	            {
					// Get group signal
	                GroupSignal = SignalGroup->ComGroupSignal[ComInitGroupSignalId];

                    /*
                       initialize each signal of n-bit sized signal type on sender and receiver side
                       with the lower n-bits of its configuration parameter ComSignalInitValue
                    */
                  
                 /* memcpy(GroupSignal->ComSignalDataPtr, config->ComSignal[ComInitGroupSignalId].ComSignalInitValue, GroupSignal->ComBitSize/8); */

                    uint8 *dest = (uint8 *) GroupSignal->ComSignalDataPtr;
                    uint8 *src =  (uint8 *) config->ComSignal[ComInitGroupSignalId].ComSignalInitValue;

                    for(int i= 0; i < GroupSignal->ComBitSize/8 ; i++)
                   {
                       *dest = src++;
                        dest++;
                   }
			       
	            }

			}


         }


}



/** 
  /brief    Send Signal
  /details  The service Com_SendSignal updates the signal object identified by SignalId with the signal referenced by the SignalDataPtr parameter.
  /param    SignalId: Id of signal to be sent.
	    SignalDataPtr: Reference to the signal data to be transmitted.
  /return   uint8: E_OK: service has been accepted.
		   COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development error).
		   COM_BUSY: in case the TP-Buffer is locked for large data types handling.
 */
uint8 Com_SendSignal (Com_SignalIdType SignalId, const void* SignalDataPtr)
{
	Com_ReturnType return_value;
	unsigned char signalMask;
	//float32 newSignalValue = *SignalDataPtr;
	float32 oldSignalValue;
	Com_IPDUType IPDUcontent = *ComIPDU;
	boolean isSignalChanged = 0;
	uint32 signalSize; 
	
	ComSignal_type* signalStruct =  GET_SIGNAL(SignalId);
	uint8 signalFilterResult = 0;
	if(signalStruct->ComSignalType == BOOLEAN)
	{
		boolean new_signalData_uint8 = *((boolean*)SignalDataPtr);
		boolean old_signalData_uint8 = *((boolean*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_uint8 != old_signalData_uint8)
		{
			isSignalChanged = 1;
		}			
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_boolean != old_signalData_boolean)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_boolean != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_boolean == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_boolean <= ComFilterMax && new_signalData_boolean >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_boolean > ComFilterMax || new_signalData_boolean < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	else if(signalStruct->ComSignalType == FLOAT32)
	{
		float32 new_signalData_float32 = *((float32*)SignalDataPtr);
		float32 old_signalData_float32 = *((float32*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_float32 != old_signalData_float32)
		{
			isSignalChanged = 1;
		}	
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_float32 != old_signalData_float32)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_float32 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_float32 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_float32 <= ComFilterMax && new_signalData_float32 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_float32 > ComFilterMax || new_signalData_float32 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	else if(signalStruct->ComSignalType == FLOAT64)
	{
		float64 new_signalData_float64 = *((float64*)SignalDataPtr);
		float64 old_signalData_float64 = *((float64*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_float64 != old_signalData_float64)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_float64 != old_signalData_float64)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_float64 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_float64 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_float64 <= ComFilterMax && new_signalData_float64 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_float64 > ComFilterMax || new_signalData_float64 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/	
	else if(signalStruct->ComSignalType == SINT16)
	{
		sint16 new_signalData_sint16 = *((sint16*)SignalDataPtr);
		sint16 old_signalData_sint16 = *((sint16*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_sint16 != old_signalData_sint16)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_sint16 != old_signalData_sint16)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_sint16 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_sint16 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_sint16 <= ComFilterMax && new_signalData_sint16 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_sint16 > ComFilterMax || new_signalData_sint16 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/	
	else if(signalStruct->ComSignalType == SINT32)
	{
		sint32 new_signalData_sint32 = *((sint32*)SignalDataPtr);
		sint32 old_signalData_sint32 = *((sint32*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_sint32 != old_signalData_sint32)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_sint32 != old_signalData_sint32)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_sint32 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_sint32 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_sint32 <= ComFilterMax && new_signalData_sint32 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_sint32 > ComFilterMax || new_signalData_sint32 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/	
	else if(signalStruct->ComSignalType == SINT64)
	{
		sint64 new_signalData_sint64 = *((sint64*)SignalDataPtr);
		sint64 old_signalData_sint64 = *((sint64*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_sint64 != old_signalData_sint64)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_sint64 != old_signalData_sint64)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_sint64 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_sint64 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_sint64 <= ComFilterMax && new_signalData_sint64 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_sint64 > ComFilterMax || new_signalData_sint64 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/	
	else if(signalStruct->ComSignalType == SINT8)
	{
		sint8 new_signalData_sint8 = *((sint8*)SignalDataPtr);
		sint8 old_signalData_sint8 = *((sint8*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_sint8 != old_signalData_sint8)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_sint8 != old_signalData_sint8)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_sint8 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_sint8 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_sint8 <= ComFilterMax && new_signalData_sint8 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_sint8 > ComFilterMax || new_signalData_sint8 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/	
	else if(signalStruct->ComSignalType == UINT16)
	{
		uint16 new_signalData_uint16 = *((uint16*)SignalDataPtr);
		uint16 old_signalData_uint16 = *((uint16*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_uint16 != old_signalData_uint16)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_uint16 != old_signalData_uint16)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_uint16 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_uint16 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_uint16 <= ComFilterMax && new_signalData_uint16 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_uint16 > ComFilterMax || new_signalData_uint16 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	else if(signalStruct->ComSignalType == UINT32)
	{
		uint32 new_signalData_uint32 = *((uint32*)SignalDataPtr);
		uint32 old_signalData_uint32 = *((uint32*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_uint32 != old_signalData_uint32)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_uint32 != old_signalData_uint32)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_uint32 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_uint32 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_uint32 <= ComFilterMax && new_signalData_uint32 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_uint32 > ComFilterMax || new_signalData_uint32 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	else if(signalStruct->ComSignalType == UINT64)
	{
		uint64 new_signalData_uint64 = *((uint64*)SignalDataPtr);
		uint64 old_signalData_uint64 = *((uint64*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_uint64 != old_signalData_uint64)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_uint64 != old_signalData_uint64)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_uint64 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_uint64 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_uint64 <= ComFilterMax && new_signalData_uint64 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_uint64 > ComFilterMax || new_signalData_uint64 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
	else if(signalStruct->ComSignalType == UINT8)
	{
		uint8 new_signalData_uint8 = *((uint8*)SignalDataPtr);
		uint8 old_signalData_uint8 = *((uint8*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
		if(new_signalData_uint8 != old_signalData_uint8)
		{
			isSignalChanged = 1;
		}
		if((signalStruct->comFilter)->ComFilterAlgorithm == ALWAYS)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && new_signalData_uint8 != old_signalData_uint8)
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_DIFFERS_MASKED_OLD) && (new_signalData_uint8 != (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && (new_signalData_uint8 == (signalStruct->comFilter)->ComFilterX) )
		{
			signalFilterResult = 1;
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_WITHIN)
		{
			if(new_signalData_uint8 <= ComFilterMax && new_signalData_uint8 >= ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( (signalStruct->comFilter)->ComFilterAlgorithm == NEW_IS_OUTSIDE)
		{
			if(new_signalData_uint8 > ComFilterMax || new_signalData_uint8 < ComFilterMin)
			{
				signalFilterResult = 1;
			}			
		}
		else if( ((signalStruct->comFilter)->ComFilterAlgorithm == MASKED_NEW_EQUALS_X) && ((signalStruct->comFilter)->ComFilterOffset == 0) )
		{
			signalFilterResult = 1;
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
	
	/*---------------------------------------------------Signal Packing in Foreground Buffer-------------------------------------------------------------------*/		
	com_foregroundBuffer[SignalId]->f_bufferDataPtr = SignalDataPtr;
	com_foregroundBuffer[SignalId]->isChangedFlag = isSignalChanged;
	
	return E_OK;
			 
}


/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_CopyShadowBufferToIPDU                                                             
 * 
 *    Parameters (in): signalGroupId
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Copy signal group data from shadow buffer to I-PDU 
 * 
 *********************************************************************************/

void Com_CopyShadowBufferToIPDU (const Com_SignalGroupIdType signalGroupId)
{
    // Get Signal Group
    const ComSignalGroup_type * SignalGroup = GET_SIGNALGROUP(signalGroupId);

    // Get I-PDU that contain this signal group
    const ComIPdu_type *IPdu = GET_IPdu(SignalGroup->ComIPduHandleId);

	
    uint8 *pduDataPtr = 0;

    if (IPdu->ComIPduDirection == RECEIVE)
	{
        pduDataPtr = IPdu->ComIPduDataPtr;
    }
    else
    {

    }
    // straight copy
    uint8 *buffer = (uint8 *)SignalGroup->ComShadowBuffer;

        for(int i= 0; i < IPdu->ComIPduLength; i++){

            *pduDataPtr = buffer++;
             pduDataPtr++;
        }

}

/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_CopyPduToShadowBuffer                                                             
 * 
 *    Parameters (in): signalGroupId
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Copy signal group data from I-PDU to shadow buffer 
 * 
 *********************************************************************************/

void Com_CopyPduToShadowBuffer(const Com_SignalGroupIdType signalGroupId) {

    // Get Signal Group
    const ComSignalGroup_type * SignalGroup = GET_SIGNALGROUP(signalGroupId);

    // Get I-PDU that contain this signal group
    const ComIPdu_type *IPdu = GET_IPdu(SignalGroup->ComIPduHandleId);
 
    const uint8 *pduDataPtr = 0;

    if (IPdu->ComIPduDirection == SEND)
    {
        pduDataPtr = IPdu->ComIPduDataPtr;
    }
    else
    {

    }

    uint8 *buffer = (uint8 *)SignalGroup->ComShadowBuffer;

    for(int i= 0; i < IPdu->ComIPduLength; i++)
    {
        *buffer++ =  *pduDataPtr++;
    }
}
