/*********************************************************************************
 *                                    Includes                                   *
 ********************************************************************************/


#include "include/Com.h"
#include "include/com_buffers.h"
#include "include/Com_Types.h"
#include "include/Com_Cfg.h"
#include "include/ComMacros.h"
#include <cstddef>
#include <minwindef.h>

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




/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_SendSignal    
 *    
 *    Service Id: 0x0a                                                             *
 * 
 *    Parameters (in): SignalId: Id of signal to be sent.
 *		       SignalDataPtr: Reference to the signal data to be transmitted.
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: uint8: 	E_OK: service has been accepted.
 *				COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was 
 *							   stopped (or service failed due
 *							   to development error).
 *				COM_BUSY: in case the TP-Buffer is locked for large data types 
 *					   handling.
 * 
 *    Description:  The service Com_SendSignal updates the signal object identified by SignalId with 
 *		    the signal referenced by the SignalDataPtr parameter.  
 * 
 *********************************************************************************/
uint8 Com_SendSignal (Com_SignalIdType SignalId, const void* SignalDataPtr)
{
	uint8 returnValue;
	if(SignalDataPtr == NULL)
	{
		returnValue = E_NOT_OK;
	}
	
	ComSignal_type* signalStruct =  GET_SIGNAL(SignalId);
	if(signalStruct == NULL)
	{
		returnValue = E_NOT_OK;
	}
	
	if(signalStruct->IsGroupSignal)
	{
		/*--------------------------------------------------------------------------------------------------------------------------------------------------------------
		If Com_SendSignal or Com_InvalidateSignal is called for a signal that belongs to a signal group, then the AUTOSAR COM will only update the shadow buffer of this
		signal group. There is no need for any further I-PDU processing like TMS evaluation, unless the I-PDU contents changed.
		[SWS_Com_00050] ⌈If Com_SendSignalGroup is called for the signal group, the AUTOSAR COM module shall copy the shadow buffer atomically to the 
		I-PDU buffer.⌋ (SRS_Com_02041)
		--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		Com_WriteSignalDataToShadowBuffer(signalStruct->SignalGroupId, signalStruct->ComHandleId, SignalDataPtr);
		returnValue = E_OK;
	}
	else
	{
		uint8 signalFilterResult = 0;
		boolean isSignalChanged = 0;
		/*---------------------------------------------------------------------------------------------------------------------------------------------------
		[SWS_Com_00602] ⌈The AUTOSAR COM module shall use filtering mechanisms on sender side for Transmission Mode Conditions 
		(TMC) but it shall not filter out signals on sender side.⌋ (SRS_Com_02083)
		---------------------------------------------------------------------------------------------------------------------------------------------------*/
		if(signalStruct->ComSignalType == BOOLEAN)
		{
			boolean new_signalData_boolean = *((boolean*)SignalDataPtr);
			boolean old_signalData_boolean = *((boolean*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_boolean, new_signalData_boolean);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}	
			*(boolean*)(signalStruct->ComFGBuffer) = new_signalData_boolean;		
		}
		else if(signalStruct->ComSignalType == FLOAT32)
		{
			float32 new_signalData_float32 = *((float32*)SignalDataPtr);
			float32 old_signalData_float32 = *((float32*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter_float(signalStruct, old_signalData_float32, new_signalData_float32);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(float32*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == FLOAT64)
		{
			float64 new_signalData_float64 = *((float64*)SignalDataPtr);
			float64 old_signalData_float64 = *((float64*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter_float(signalStruct, old_signalData_float64, new_signalData_float64);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(float64*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == SINT16)
		{
			sint16 new_signalData_sint16 = *((sint16*)SignalDataPtr);
			sint16 old_signalData_sint16 = *((sint16*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint16, new_signalData_sint16);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(sint16*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == SINT32)
		{
			sint32 new_signalData_sint32 = *((sint32*)SignalDataPtr);
			sint32 old_signalData_sint32 = *((sint32*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint32, new_signalData_sint32);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(sint32*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == SINT64)
		{
			sint64 new_signalData_sint64 = *((sint64*)SignalDataPtr);
			sint64 old_signalData_sint64 = *((sint64*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint64, new_signalData_sint64);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(sint64*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}	
		else if(signalStruct->ComSignalType == SINT8)
		{
			sint8 new_signalData_sint8 = *((sint8*)SignalDataPtr);
			sint8 old_signalData_sint8 = *((sint8*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint8, new_signalData_sint8);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(sint8*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == UINT16)
		{
			uint16 new_signalData_uint16 = *((uint16*)SignalDataPtr);
			uint16 old_signalData_uint16 = *((uint16*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint16, new_signalData_uint16);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(uint16*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == UINT32)
		{
			uint32 new_signalData_uint32 = *((uint32*)SignalDataPtr);
			uint32 old_signalData_uint32 = *((uint32*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint32, new_signalData_uint32);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(uint32*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == UINT64)
		{
			uint64 new_signalData_uint64 = *((uint64*)SignalDataPtr);
			uint64 old_signalData_uint64 = *((uint64*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint64, new_signalData_uint64);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(uint64*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
		else if(signalStruct->ComSignalType == UINT8)
		{
			uint8 new_signalData_uint8 = *((uint8*)SignalDataPtr);
			uint8 old_signalData_uint8 = *((uint8*)(com_foregroundBuffer[SignalId]->f_bufferDataPtr));
			signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint8, new_signalData_uint8);
			if(new_signalData_boolean != old_signalData_boolean)
			{
				isSignalChanged = 1;
			}
			*(uint8*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
		}
	
		/*---------------------------------------------------Pack signal processed info in signal struct----------------------------------------------------*/		
		signalStruct->ComSignalFilterResult = signalFilterResult;
	
		/*---------------------------------------------------------------------------------------------------------------------------------------------------
		[SWS_Com_00734] ⌈At a send request of a signal with ComTransferProperty TRIGGERED_ON_CHANGE assigned to an I-PDU with ComTxModeMode DIRECT or MIXED, 
		the AUTOSAR COM module shall immediately (within the next main function at the latest) initiate ComTxModeNumberOfRepetitions plus one transmissions of 
		the assigned I-PDU, if the new sent signal differs to the locally stored (last sent or init) in length or value.⌋ (SRS_Com_02083)
		---------------------------------------------------------------------------------------------------------------------------------------------------*/
		signalStruct->ComIsSignalChanged = isSignalChanged;
		
		/*--------------------------------------------------------------------------------------------------------------------------------------------------------------
		[SWS_Com_00061] ⌈If the RTE updates the value of a signal by calling Com_SendSignal, the AUTOSAR COM module shall set the update-bit of this signal.⌋ 
		(SRS_Com_02030)
		---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		ComIPdu_type* Ipdu = GET_IPDU(signalStruct->ComIPduHandleId);
		uint64 updatebitMask = 1u << (signalStruct->ComUpdateBitPosition);
		*(uint64*)(Ipdu->ComIPduDataPtr) = *(uint64*)(Ipdu->ComIPduDataPtr) | updatebitMask;
		
		returnValue = E_OK;
	}
	
	
	
	return returnValue;
			 
}



/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_EnableReceptionDM   
 *    
 *    Service Id: 0x06                                                          
 * 
 *    Parameters (in): IpduGroupId
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Enables the reception deadline monitoring for the I-PDUs
 *                  within the given IPDU group.
 * 
 *********************************************************************************/
void Com_EnableReceptionDM (Com_IpduGroupIdType IpduGroupId)
{   
     uint16 ipduIndex;
     uint16  ComSignalIndex ;

     ComIPduGroup_type *ipduGroup;
     ComIPdu_type *IPdu;
     ComSignal_type *Signal;

    ipduGroup = GET_IpduGroup(IpduGroupId);

      if (ipduGroup != NULL) 
      {
         for (ipduIndex = 0; ipduIndex < ipduGroup->numIPdus; ipduIndex++)
          {
            /*
              [SWS_Com_00534] If Com_EnableReceptionDM is invoked on an I-PDU group
              containing Tx-I-PDUs, then the AUTOSAR COM module shall silently ignore 
              this request.
            */
            if(ipduGroup->IPDU[ipduIndex].ComIPduDirection != SEND)
            {
            /*
              [SWS_Com_00486] The AUTOSAR COM module shall silently ignore setting the
              reception deadline monitoring of an I-PDU to enabled by Com_EnableReceptionDM,
              in case the reception deadline monitoring is already enabled for this I-PDU
            */
               if(!ipduGroup->IPDU[ipduIndex].ReceptionDMEnabled)
               {
                  ipduGroup->IPDU[ipduIndex].ReceptionDMEnabled = TRUE;
                
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

/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_DisableReceptionDM   
 *    
 *    Service Id: 0x05                                                          
 * 
 *    Parameters (in): IpduGroupId
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Disables the reception deadline monitoring for the I-PDUs
 *                  within the given IPDU group.
 *********************************************************************************/
void Com_DisableReceptionDM (Com_IpduGroupIdType IpduGroupId)
{   
     uint16 ipduIndex;
     uint16  ComSignalIndex ;

     ComIPduGroup_type *ipduGroup;
     ComIPdu_type *IPdu;
     ComSignal_type *Signal;

    ipduGroup = GET_IpduGroup(IpduGroupId);

      if (ipduGroup != NULL) 
      {
         for (ipduIndex = 0; ipduIndex < ipduGroup->numIPdus; ipduIndex++)
          {
            /*
              [SWS_Com_00534] If Com_DisableReceptionDM is invoked on an I-PDU group
              containing Tx-I-PDUs, then the AUTOSAR COM module shall silently ignore 
              this request.
            */
            if(ipduGroup->IPDU[ipduIndex].ComIPduDirection != SEND)
            {
            /*
              [SWS_Com_00486] The AUTOSAR COM module shall silently ignore setting the
              reception deadline monitoring of an I-PDU to enabled by Com_EnableReceptionDM,
              in case the reception deadline monitoring is already enabled for this I-PDU
            */
               if(!ipduGroup->IPDU[ipduIndex].ReceptionDMEnabled)
               {
                  ipduGroup->IPDU[ipduIndex].ReceptionDMEnabled = FALSE;
                
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
 uint8 Com_ReceiveSignalGroup (Com_SignalGroupIdType SignalGroupId)
 {
    const ComSignalGroup_Type * SignalGroup= GET_SIGNALGROUP(GroupSignal->SignalGroupId);
    const ComIPdu_type *Ipdu=GET_IPDU(SignalGroup->ComIPduHandleId);
	if(Ipdu->ComIPduDirection!=RECEIVE)
	{
		return E_NOT_OK;
	}
	else
	{
       if(Ipdu->ComIPduGroupRef->IpduGroupFlag==STOPPED)
    {
        /*[SWS_Com_00461] ⌈The AUTOSAR COM module shall always copy the last known 
         data, or the ComSignalInitValue(s) if not yet written, of the I-PDU to the shadow buffer by a call to Com_ReceiveSignalGroup even if the I-PDU is stopped and COM_-
        SERVICE_NOT_AVAILABLE is returned*/
        CopySignalGroupfromBGtoSB( SignalGroupId);
        return COM_SERVICE_NOT_AVAILABLE;
    }
     else
    {
        CopySignalGroupfromBGtoSB( SignalGroupId);
        return E_OK;
    }

	}
    
 }
 void Com_RxIndication (PduIdType RxPduId, const PduInfoType* PduInfoPtr)
 {

 }
 uint8 Com_ReceiveSignal (Com_SignalIdType SignalId, void* SignalDataPtr)
 {
	
	
   if(SignalId>=0&&SignalId<=32767)
   {
    const ComSignal_Type * Signal=GET_SIGNAL(SignalId);
	if(signal->containingIPDU->ComIPduDirection!=RECEIVE)
	{
		return E_NOT_OK;
	}
	else
	{
		if(signal->containingIPDU->ComIPduGroupRef->IpduGroupFlag==STOPPED)
    {
        return COM_SERVICE_NOT_AVAILABLE;
    }
    else
    {    //CopySignalfromBGtoFG(SignalId);
         CopySignalFromFGtoAddress(SignalId,SignalDataPtr);
        return E_OK;
    }
   }
	}
    
   else if(SignalId>32767&&SignalId<=65535)
   {
    const ComGroupSignal_type * GroupSignal=GET_GROUPSIGNAL(SignalId);
    const ComSignalGroup_Type * SignalGroup= GET_SIGNALGROUP(GroupSignal->SignalGroupId);
    const ComIPdu_type *Ipdu=GET_IPDU(SignalGroup->ComIPduHandleId);
	if(Ipdu->ComIPduDirection!=RECEIVE)
	{
		return E_NOT_OK;
	}
	else
	{
        if(Ipdu->ComIPduGroupRef->IpduGroupFlag==STOPPED)
    {
        return COM_SERVICE_NOT_AVAILABLE;
    }
    else
    {
         CopyGroupSignalFromSBtoAddress(GroupSignal->SignalGroupId,SignalDataPtr);
        return E_OK;
    }
	}
     
   }
   else
   {
	return E_NOT_OK;
   }
 }

