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
 void Com_RxIndication (PduIdType RxPduId, const PduInfoType* PduInfoPtr)
 {

 }
 uint8 Com_ReceiveSignal (Com_SignalIdType SignalId, void* SignalDataPtr)
 {
   if(SignalId>=0&&SignalId<=32767)
   {
    const ComSignal_Type * Signal=GET_SIGNAL(SignalId);
    if(signal->containingIPDU->ComIPduGroupRef->IpduGroupFlag==STOPPED)
    {
        return COM_SERVICE_NOT_AVAILABLE;
    }
    else
    {    CopySignalfromBGtoFG(SignalId);
         CopySignalFromFGtoAddress(SignalId,SignalDataPtr);
        return E_OK;
    }
   }
   else
   {
    const ComGroupSignal_type * GroupSignal=GET_GROUPSIGNAL(SignalId);
    const ComSignalGroup_Type * SignalGroup= GET_SIGNALGROUP(GroupSignal->SignalGroupId);
    const ComIPdu_type *Ipdu=GET_IPDU(SignalGroup->ComIPduHandleId);
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
