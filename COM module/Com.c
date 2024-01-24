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
void * foreGroundBuffer[65536];
void * ShadowBuffer[65536];
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
/***********************************************************************************
 *                                                                                 *
 *    Service Name:  Com_ReceiveSignal                                                            
 * 
 *    Parameters (in): SignalId Id of signal to be received
 * 
 *    Parameters (out): SignalDataPtrReference to the location where the received signal data shall be stored

 * 
 *    Return Value:  uint8
 * 
 *    Description:  Com_ReceiveSignal copies the data of the signal identified by SignalId to the location specified by SignalDataPtr.
 * 
 *********************************************************************************/
 uint8 Com_ReceiveSignal (Com_SignalIdType SignalId, void* SignalDataPtr)
 {
   ComSignalType_type type = GET_SIGNAL(SignalId)->ComSignalType;
    switch(type){
    case BOOLEAN:
    if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(boolean*) SignalDataPtr= *(boolean*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(boolean*) SignalDataPtr= *(boolean*)ShadowBuffer[SignalId];
    }
    break;
    case FLOAT32:
     if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(float32*) SignalDataPtr= *(float32*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(float32*) SignalDataPtr= *(float32*)ShadowBuffer[SignalId];
    }
    break;
    case FLOAT64:
     if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(float64*) SignalDataPtr= *(float64*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(float64*) SignalDataPtr= *(float64*)ShadowBuffer[SignalId];
    }
    break;
    case SINT16:
     if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(sint16*) SignalDataPtr= *(sint16*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(sint16*) SignalDataPtr= *(sint16*)ShadowBuffer[SignalId];
    }
    break;
    case SINT32:
      if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(sint32*) SignalDataPtr= *(sint32*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(sint32*) SignalDataPtr= *(sint32*)ShadowBuffer[SignalId];
    }
    break;

    case SINT64:
      if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(sint64*) SignalDataPtr= *(sint64*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(sint64*) SignalDataPtr= *(sint64*)ShadowBuffer[SignalId];
    }
    break;
    case SINT8:
      if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(sint8*) SignalDataPtr= *(sint8*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(sint8*) SignalDataPtr= *(sint8*)ShadowBuffer[SignalId];
    }
    break;
    case UINT16:
      if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(uint16*) SignalDataPtr= *(uint16*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(uint16*) SignalDataPtr= *(uint16*)ShadowBuffer[SignalId];
    }
    break;
    case UINT32:
        if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(uint32*) SignalDataPtr= *(uint32*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(uint32*) SignalDataPtr= *(uint32*)ShadowBuffer[SignalId];
    }
    break;
    case UINT64:
        if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(uint64*) SignalDataPtr= *(uint64*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(uint64*) SignalDataPtr= *(uint64*)ShadowBuffer[SignalId];
    }
    break;
    case UINT8:
        if(foreGroundBuffer[SignalId]!=NULL)
    {
        *(uint8*) SignalDataPtr= *(uint8*)foreGroundBuffer[SignalId];
    }
    else
    {
        *(uint8*) SignalDataPtr= *(uint8*)ShadowBuffer[SignalId];
    }
    break;
    default:
    break;
    }
    return E_OK;
 }
 	
