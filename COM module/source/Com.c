/*********************************************************************************
 *                                    Includes                                   *
 ********************************************************************************/


#include "../include/Com.h"
#include "../include/Com_HelpingFunctions.h"
#include "../include/com_buffers.h"
#include "../include/Com_Types.h"
#include "../include/Com_Cfg.h"
#include "../include/ComMacros.h"
#include "../../Det/inc/Det.h"
#include "../../Det/inc/Com_Det.h"
#include "../libraries/Std_Types.h"
#include <string.h>
#include <cstddef>
//#include <minwindef.h>

static Com_StatusType initStatus = COM_UNINIT;

//const Com_ConfigType * ComConfig;

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



void Com_Init (const Com_ConfigType* config)
{
				
	    ComIPdu_type *IPdu;
    	ComSignal_type *Signal;
		ComSignalGroup_type *SignalGroup;
	    ComGroupSignal_type *GroupSignal;

	    uint8 ComInitPduId;
		uint8 ComInitSignalId;
        uint8 ComInitGroupSignalId;
        uint8 ComInitSignalGroupId;

		uint8 *ComShadowBuffer;
	
        // Initialize global and static variables
        ComConfig = config;
    

	    // Loop over all I-PDUs
        for (ComInitPduId = 0; ComInitPduId < COM_NUM_OF_IPDU ; ComInitPduId++)
		 {
            
			// Get IPdu
		    IPdu = GET_IPdu(ComInitPduId);
	
          /*[SWS_Com_00015] The AUTOSAR COM module shall fill not used areas within an
            I-PDU with a value determined by configuration parameter ComTxIPduUnusedAreasDefault*/
	       if (IPdu->ComIPduDirection == SEND)
		    {
			   memset((void *)IPdu->ComIPduDataPtr, IPdu->ComTxIPdu->ComTxIPduUnusedAreasDefault, IPdu->ComIPduLength);
	    	}
			else
			{

			}
			/*[SWS_Com_00444] ⌈By default, all I-PDU groups shall be in the state stopped and
               they shall not be started automatically by a call to Com_Init*/
			if (IPdu->ComIPduGroupRef != NULL)
			{
				IPdu->ComIPduGroupRef->IpduGroupFlag = STOPPED;
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
              memcpy(Signal->ComSignalDataPtr, config->ComSignal[ComInitSignalId].ComSignalInitValue, Signal->ComBitSize/8);


              /*uint8 *dest = (uint8 *) Signal->ComSignalDataPtr;
               uint8 *src =  (uint8 *) config->ComSignal[ComInitSignalId].ComSignalInitValue;
				       

                    for(int i= 0; i < (Signal->ComBitSize)/8 ; i++)
                   {
                       *dest = src++;
                        dest++;
                   }
				   */
               //[SWS_Com_00117] The AUTOSAR COM module shall clear all update-bits during initialization
			   CLEARBIT(IPdu->ComIPduDataPtr, Signal->ComUpdateBitPosition);


			}
            // For each signal group at the I-PDU
		    for (ComInitSignalGroupId = 0; IPdu[ComInitSignalGroupId].ComIPduSignalGroupRef[ComInitSignalGroupId] != NULL; ComInitSignalGroupId++)
            {
                /* Get SignalGroup */
                SignalGroup = IPdu[ComInitSignalGroupId].ComIPduSignalGroupRef[ComInitSignalGroupId];
    
                //[SWS_Com_00117] The AUTOSAR COM module shall clear all update-bits during initialization
                CLEARBIT(IPdu->ComIPduDataPtr, SignalGroup->ComUpdateBitPosition);
                
				/*[SWS_Com_00484] By a call to Com_Init, the AUTOSAR COM module shall initialize 
				  the shadow buffer of a signal group on sender-side*/
	            ComShadowBuffer = (uint8*)SignalGroup->ComShadowBuffer;
            

                // For each group signal at signal group
                for(ComInitGroupSignalId=0 ;(SignalGroup->ComGroupSignal != NULL && SignalGroup->ComGroupSignal[ComInitGroupSignalId] != NULL) ; ComInitGroupSignalId++)
	            {
					// Get group signal
	                GroupSignal = SignalGroup->ComGroupSignal[ComInitGroupSignalId];

                    /*
                       initialize each signal of n-bit sized signal type on sender and receiver side
                       with the lower n-bits of its configuration parameter ComSignalInitValue
                    */
                  
                  memcpy(GroupSignal->ComSignalDataPtr, config->ComSignal[ComInitGroupSignalId].ComSignalInitValue, GroupSignal->ComBitSize/8); 
                   /*
                    uint8 *dest = (uint8 *) GroupSignal->ComSignalDataPtr;
                    uint8 *src =  (uint8 *) config->ComSignal[ComInitGroupSignalId].ComSignalInitValue;

                    for(int i= 0; i < GroupSignal->ComBitSize/8 ; i++)
                   {
                       *dest = src++;
                        dest++;
                   }
			       */
	            }

			}


         }
   initStatus = COM_INIT;

}


/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_DeInit    
 *    
 *    Service Id: 0x02                                                             *
 * 
 *    Parameters (in):  None
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description: This service stops the inter-ECU communication.
 *                 All started I-PDU groups are stopped and have to 
 *                 be started again, if needed, after Com_Init is called. 
 *                 By a call to Com_DeInit the AUTOSAR COM module is put
 *                 into an not initialized state.
 * 
 *********************************************************************************/

void Com_DeInit( void ) 
{
	ComIPdu_type *IPdu;
	uint8 ComDeInitPduId;

    if( initStatus != COM_INIT ) 
	{

        /*Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_DEINIT_ID, COM_E_UNINIT);*/

        return;
    }
    
    for (ComDeInitPduId = 0; ComDeInitPduId < COM_NUM_OF_IPDU ; ComDeInitPduId++)
		 { 
			// Get IPdu
		    IPdu = GET_IPdu(ComDeInitPduId);

			if(IPdu->ComIPduGroupRef != NULL)
			{
				Com_IpduGroupStop(IPdu->ComIPduGroupRef->ComIPduGroupHandleId);

			}
			else
			{
				// NULL pointer (IPdu does not belongs to any IPdu group)
				Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, COM_DEINIT_ID, COM_E_PARAM_POINTER);
			}

         }
    initStatus = COM_UNINIT;
}




/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_IpduGroupStart   
 *    
 *    Service Id: 0x03                                                         
 * 
 *    Parameters (in): IpduGroupId
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Starts a preconfigured I-PDU group.
 *********************************************************************************/
void Com_IpduGroupStart(Com_IpduGroupIdType IpduGroupId , boolean Initialize) 
{
	ComIPduGroup_type * IPduGroup;
    ComSignal_type *Signal = NULL;
	ComIPdu_type * IPdu;

	uint8 IpduId;
	uint8 ComSignalId;

	float MDT ;


	IPduGroup = GET_IpduGroup(IpduGroupId);

    for (IpduId = 0; IpduId < COM_NUM_OF_IPDU ; IpduId++)
		 {

		    IPdu = GET_IPdu(IpduId);

			if(IPdu->ComIPduGroupRef != NULL)

			{
				/*[SWS_Com_00787] If an I-PDU is started by Com_IpduGroupStart, the AUTOSAR
                COM module shall always initialize the following attributes of this I-PDU:
                
                3) cancel all transmission deadline monitoring timers and use ComFirstTimeout (if
                configured) as value when a transmission timer is started the first time after the
                I-PDU activation
              
                5) reset OCCURRENCE of filters with ComFilterAlgorithm ONE_EVERY_N
                6) set the I-PDU counter to 0 for I-PDUs with ComIPduDirection configured to
                SEND
                7) accept for I-PDUs with ComIPduDirection configured to RECEIVED any next
                incoming I-PDU counter*/
				if(IPdu->ComIPduGroupRef->ComIPduGroupHandleId == IpduGroupId)
				{
					//1) ComMinimumDelayTime of I-PDUs in transmission mode DIRECT or MIXED
					IPdu->ComTxIPdu->ComMinimumDelayTime = MDT;

					for (ComSignalId = 0; (IPdu->ComIPduSignalRef[ComSignalId] != NULL); ComSignalId++)
                    {
                        //Get signal
                        Signal = IPdu->ComIPduSignalRef[ComSignalId];
                        // 2) restart all reception deadline monitoring timers for all signals with a non-zero configured ComFirstTimeout
						if (Signal->ComFirstTimeout > 0)
						{
							Signal->DeadlineMonitoringTimer = Signal->ComFirstTimeout;
						}
                        // 4) all included update-bits shall be cleared
						CLEARBIT(IPdu->ComIPduDataPtr, Signal->ComUpdateBitPosition);
		
          
                    }
				

				}
				else
				{

				}
			}
			else
			{

			}

		 }

	if(IPduGroup != NULL)
	{
		IPduGroup->IpduGroupFlag = STARTED;
	}
}

/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_IpduGroupStop   
 *    
 *    Service Id: 0x04                                                         
 * 
 *    Parameters (in): IpduGroupId
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Stops a preconfigured I-PDU group.
 *********************************************************************************/
void Com_IpduGroupStop(Com_IpduGroupIdType IpduGroupId)
 {
	ComIPduGroup_type * IPduGroup; 

	IPduGroup = GET_IpduGroup(IpduGroupId);

	if(IPduGroup != NULL)
	{
		IPduGroup->IpduGroupFlag = STOPPED;
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
         for (ipduIndex = 0; ipduIndex < COM_NUM_OF_IPDU; ipduIndex++)
          {
			  IPdu = GET_IPdu(ipduIndex);
	
            /*
              [SWS_Com_00534] If Com_EnableReceptionDM is invoked on an I-PDU group
              containing Tx-I-PDUs, then the AUTOSAR COM module shall silently ignore 
              this request.
            */
            if(IPdu->ComIPduGroupRef->ComIPduGroupHandleId == IpduGroupId)
            {
				if(IPdu->ComIPduDirection != SEND)
				{
					 if(!IPdu[ipduIndex].ReceptionDMEnabled)
                     {
                        IPdu[ipduIndex].ReceptionDMEnabled = 1;
                
                     }
                     else
                    {

                    }


				}
            /*
              [SWS_Com_00486] The AUTOSAR COM module shall silently ignore setting the
              reception deadline monitoring of an I-PDU to enabled by Com_EnableReceptionDM,
              in case the reception deadline monitoring is already enabled for this I-PDU
            */
              
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
         for (ipduIndex = 0; ipduIndex < COM_NUM_OF_IPDU; ipduIndex++)
          {
			  IPdu = GET_IPdu(ipduIndex);
	
            /*
              [SWS_Com_00534] If Com_EnableReceptionDM is invoked on an I-PDU group
              containing Tx-I-PDUs, then the AUTOSAR COM module shall silently ignore 
              this request.
            */
            if(IPdu->ComIPduGroupRef->ComIPduGroupHandleId == IpduGroupId)
            {
				if(IPdu->ComIPduDirection != SEND)
				{
					 if(!IPdu[ipduIndex].ReceptionDMEnabled)
                     {
                        IPdu[ipduIndex].ReceptionDMEnabled = 0;
                
                     }
                     else
                    {

                    }


				}
            /*
              [SWS_Com_00486] The AUTOSAR COM module shall silently ignore setting the
              reception deadline monitoring of an I-PDU to enabled by Com_EnableReceptionDM,
              in case the reception deadline monitoring is already enabled for this I-PDU
            */
              
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
 *    Description:  The service Com_SendSignal updates the signal object identified 
 *                  by SignalId with the signal referenced by the SignalDataPtr parameter.  
 * 
 *********************************************************************************/

uint8 Com_SendSignal (Com_SignalIdType SignalId, const void* SignalDataPtr)
{
	uint8 returnValue;
	if(Com_GetStatus() == COM_UNINIT || SignalDataPtr == NULL)
	{
		returnValue = COM_SERVICE_NOT_AVAILABLE;
	}
	else
	{
		if(SignalId < COM_MIN_SIGNAL)
		{
					ComGroupSignal_type* GroupSignalStruct =  GET_GROUPSIGNALCNFG(SignalId);
					/*--------------------------------------------------------------------------------------------------------------------------------------------------------------
					If Com_SendSignal or Com_InvalidateSignal is called for a signal that belongs to a signal group, then the AUTOSAR COM will only update the shadow buffer of this
					signal group. There is no need for any further I-PDU processing like TMS evaluation, unless the I-PDU contents changed.
					[SWS_Com_00050] ⌈If Com_SendSignalGroup is called for the signal group, the AUTOSAR COM module shall copy the shadow buffer atomically to the 
					I-PDU buffer.⌋ (SRS_Com_02041)
					--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
					CopyGroupSignalFromSBtoAddress(GroupSignalStruct->SignalGroupId, GroupSignalStruct->ComHandleId, GroupSignalStruct->ComSignalDataPtr);
					Com_WriteSignalDataToShadowBuffer(GroupSignalStruct->SignalGroupId, GroupSignalStruct->ComHandleId, SignalDataPtr);
					returnValue = E_OK;
		}
		else
		{
			ComSignal_type* signalStruct =  GET_SIGNAL(SignalId);
			if(signalStruct == NULL)
			{
				returnValue = COM_SERVICE_NOT_AVAILABLE;
			}
			else
			{
				ComIPdu_type* Ipdu = GET_IPDU(signalStruct->ComIPduHandleId);
				if(Ipdu == NULL)
				{
					returnValue = COM_SERVICE_NOT_AVAILABLE;
				}
				else
				{
							
							boolean signalFilterResult = 0;
							boolean isSignalChanged = 0;
							uint8 byteOffset, bitOffset, updatebitMask, *pduBufferPointer;
							boolean new_signalData_boolean, old_signalData_boolean;
							float32 new_signalData_float32, old_signalData_float32;
							float64 new_signalData_float64, old_signalData_float64;
							sint16 new_signalData_sint16, old_signalData_sint16;
							sint32 new_signalData_sint32, old_signalData_sint32;
							sint64 new_signalData_sint64, old_signalData_sint64;
							sint8 new_signalData_sint8, old_signalData_sint8;
							uint8 new_signalData_uint8, old_signalData_uint8;
							uint16 new_signalData_uint16, old_signalData_uint16;
							uint32 new_signalData_uint32, old_signalData_uint32;
							uint64 new_signalData_uint64, old_signalData_uint64;
							/*---------------------------------------------------------------------------------------------------------------------------------------------------
							[SWS_Com_00602] ⌈The AUTOSAR COM module shall use filtering mechanisms on sender side for Transmission Mode Conditions 
							(TMC) but it shall not filter out signals on sender side.⌋ (SRS_Com_02083)
							---------------------------------------------------------------------------------------------------------------------------------------------------*/
							switch(signalStruct->ComSignalType)
							{
								case BOOLEAN:
									old_signalData_boolean = *((boolean*)(signalStruct->ComFGBuffer));
									new_signalData_boolean = *((boolean*)SignalDataPtr);
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_boolean, new_signalData_boolean);
									if(new_signalData_boolean != old_signalData_boolean)
									{
										isSignalChanged = 1;
									}
									else{}
									*(boolean*)(signalStruct->ComFGBuffer) = new_signalData_boolean;
									break;
								
								case FLOAT32:
								
									new_signalData_float32 = *((float32*)SignalDataPtr);
									old_signalData_float32 = *((float32*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter_float(signalStruct, old_signalData_float32, new_signalData_float32);
									if(new_signalData_float32 != old_signalData_float32)
									{
										isSignalChanged = 1;
									}
									else{}
									*(float32*)(signalStruct->ComFGBuffer) = new_signalData_float32;
									break;
								
								case FLOAT64:
								
									new_signalData_float64 = *((float64*)SignalDataPtr);
									old_signalData_float64 = *((float64*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter_float(signalStruct, old_signalData_float64, new_signalData_float64);
									if(new_signalData_float64 != old_signalData_float64)
									{
										isSignalChanged = 1;
									}
									else{}
									*(float64*)(signalStruct->ComFGBuffer) = new_signalData_float64;
									break;
								
								case SINT16:
								
									new_signalData_sint16 = *((sint16*)SignalDataPtr);
									old_signalData_sint16 = *((sint16*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint16, new_signalData_sint16);
									if(new_signalData_sint16 != old_signalData_sint16)
									{
										isSignalChanged = 1;
									}
									else{}
									*(sint16*)(signalStruct->ComFGBuffer) = new_signalData_sint16;
									break;
								
								case SINT32:
								
									new_signalData_sint32 = *((sint32*)SignalDataPtr);
									old_signalData_sint32 = *((sint32*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint32, new_signalData_sint32);
									if(new_signalData_sint32 != old_signalData_sint32)
									{
										isSignalChanged = 1;
									}
									else{}
									*(sint32*)(signalStruct->ComFGBuffer) = new_signalData_sint32;
									break;
								
								case SINT64:
								
									new_signalData_sint64 = *((sint64*)SignalDataPtr);
									old_signalData_sint64 = *((sint64*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint64, new_signalData_sint64);
									if(new_signalData_sint64 != old_signalData_sint64)
									{
										isSignalChanged = 1;
									}
									else{}
									*(sint64*)(signalStruct->ComFGBuffer) = new_signalData_sint64;
									break;
								
								case SINT8:
								
									new_signalData_sint8 = *((sint8*)SignalDataPtr);
									old_signalData_sint8 = *((sint8*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_sint8, new_signalData_sint8);
									if(new_signalData_sint8 != old_signalData_sint8)
									{
										isSignalChanged = 1;
									}
									else{}
									*(sint8*)(signalStruct->ComFGBuffer) = new_signalData_sint8;
									break;
								
								case UINT16:
								
									new_signalData_uint16 = *((uint16*)SignalDataPtr);
									old_signalData_uint16 = *((uint16*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint16, new_signalData_uint16);
									if(new_signalData_uint16 != old_signalData_uint16)
									{
										isSignalChanged = 1;
									}
									else{}
									*(uint16*)(signalStruct->ComFGBuffer) = new_signalData_uint16;
									break;
								
								case UINT32:
								
									new_signalData_uint32 = *((uint32*)SignalDataPtr);
									old_signalData_uint32 = *((uint32*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint32, new_signalData_uint32);
									if(new_signalData_uint32 != old_signalData_uint32)
									{
										isSignalChanged = 1;
									}
									else{}
									*(uint32*)(signalStruct->ComFGBuffer) = new_signalData_uint32;
									break;
								
								case UINT64:
								
									new_signalData_uint64 = *((uint64*)SignalDataPtr);
									old_signalData_uint64 = *((uint64*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint64, new_signalData_uint64);
									if(new_signalData_uint64 != old_signalData_uint64)
									{
										isSignalChanged = 1;
									}
									else{}
									*(uint64*)(signalStruct->ComFGBuffer) = new_signalData_uint64;
									break;
								
								case UINT8:
								
									new_signalData_uint8 = *((uint8*)SignalDataPtr);
									old_signalData_uint8 = *((uint8*)(signalStruct->ComFGBuffer));
									signalFilterResult = Com_ProcessTxSignalFilter(signalStruct, old_signalData_uint8, new_signalData_uint8);
									if(new_signalData_uint8 != old_signalData_uint8)
									{
										isSignalChanged = 1;
									}
									else{}
									*(uint8*)(signalStruct->ComFGBuffer) = new_signalData_uint8;
									break;
							}
							
							/*---------------------------------------------------Pack signal processed info in signal struct----------------------------------------------------*/		
							signalStruct->ComSignalFilterResult = signalFilterResult;
			
							signalStruct->ComIsSignalChanged = isSignalChanged;
				
							/*--------------------------------------------------------------------------------------------------------------------------------------------------------------
							[SWS_Com_00061] ⌈If the RTE updates the value of a signal by calling Com_SendSignal, the AUTOSAR COM module shall set the update-bit of this signal.⌋ 
							(SRS_Com_02030)
							---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
							byteOffset = signalStruct->ComUpdateBitPosition /8;
							bitOffset = signalStruct->ComUpdateBitPosition %8 ;
							
							pduBufferPointer = (uint8*)Ipdu->ComIPduDataPtr;
							pduBufferPointer += byteOffset;
							updatebitMask = 1u << (bitOffset);
							*pduBufferPointer = *pduBufferPointer | updatebitMask;
							/*-----------------------------------------------IPDU Transmission Mode Selection---------------------------------------------------------*/
							Ipdu->ComTxIPdu->ComCurrentTransmissionSelection = com_pdu_transmissionsModeSelection(Ipdu);
							/*--------------------------Handle required number of transmissions of IPDU according to this signal--------------------------------------*/
							if(com_pdu_transmissions_handle_signal(Ipdu, signalStruct) == E_NOT_OK)
							{
								returnValue = COM_SERVICE_NOT_AVAILABLE;
							}
							else
							{
								returnValue = E_OK;
							}
						
						}
			}
			
			}
	}			
			
	return returnValue;
			 
}



/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_SendSignalGroup    
 *    
 *    Service Id: 0x0d                                                             *
 * 
 *    Parameters (in): SignalGroupId: Id of signal group to be sent.
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
 *    Description:  The service Com_SendSignalGroup copies the content of the
                     associated shadow buffer to the foreground buffer.  
 * 
 *********************************************************************************/

uint8 Com_SendSignalGroup (Com_SignalGroupIdType SignalGroupId)
{
	/* 1- copy all group signal data from shadow buffer to foreground buffer
	   2- check filter
	   3- check if at least one signal changed
	   4- set update bit of signal group
	*/
	uint8 returnValue;
	if(Com_GetStatus() == COM_UNINIT)
	{
		returnValue = COM_SERVICE_NOT_AVAILABLE;
	}
	else
	{
		ComSignalGroup_type* signalGroup = GET_SIGNALGROUP(SignalGroupId);
		if(signalGroup == NULL)
		{
			returnValue = COM_SERVICE_NOT_AVAILABLE;
		}
		else
		{
			ComIPdu_type* Ipdu = GET_IPDU(signalGroup->ComIPduHandleId);
			if(Ipdu == NULL)
			{
				returnValue = COM_SERVICE_NOT_AVAILABLE;
			}
			else
			{
				boolean isSignalGroupChanged = 0;
				boolean SignalGroupFilterResult = 0;
				boolean new_signalData_boolean, old_signalData_boolean;
				float32 new_signalData_float32, old_signalData_float32;
				float64 new_signalData_float64, old_signalData_float64;
				sint16 new_signalData_sint16, old_signalData_sint16;
				sint32 new_signalData_sint32, old_signalData_sint32;
				sint64 new_signalData_sint64, old_signalData_sint64;
				sint8 new_signalData_sint8, old_signalData_sint8;
				uint8 new_signalData_uint8, old_signalData_uint8;
				uint16 new_signalData_uint16, old_signalData_uint16;
				uint32 new_signalData_uint32, old_signalData_uint32;
				uint64 new_signalData_uint64, old_signalData_uint64;
				uint16 i;
				for(i=0; (signalGroup->ComGroupSignal)[i] != NULL; i++)
				{
					ComGroupSignal_type* groupSignal = (signalGroup->ComGroupSignal)[i];
					
					/*---------------------------------------------------------------------------------------------------------------------------------------------------
					[SWS_Com_00602] ⌈The AUTOSAR COM module shall use filtering mechanisms on sender side for Transmission Mode Conditions 
					(TMC) but it shall not filter out signals on sender side.⌋ (SRS_Com_02083)
					---------------------------------------------------------------------------------------------------------------------------------------------------*/
					switch(groupSignal->ComSignalType)
					{
						case BOOLEAN:
					
							old_signalData_boolean = *((boolean*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_boolean);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_boolean, new_signalData_boolean);
							if(new_signalData_boolean != old_signalData_boolean)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(boolean*)(groupSignal->ComFGBuffer) = new_signalData_boolean;
							break;
					
						case FLOAT32:
					
							old_signalData_float32 = *((float32*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_float32);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_float_groupsignal(groupSignal, old_signalData_float32, new_signalData_float32);
							if(old_signalData_float32 != new_signalData_float32)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(float32*)(groupSignal->ComFGBuffer) = new_signalData_float32;
							break;
					
						case FLOAT64:
					
							old_signalData_float64 = *((float64*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_float64);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_float_groupsignal(groupSignal, old_signalData_float64, new_signalData_float64);
							if(old_signalData_float64 != new_signalData_float64)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(float64*)(groupSignal->ComFGBuffer) = new_signalData_float64;
							break;
					
						case SINT16:
					
							old_signalData_sint16 = *((sint16*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_sint16);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_sint16, new_signalData_sint16);
							if(old_signalData_sint16 != new_signalData_sint16)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(sint16*)(groupSignal->ComFGBuffer) = new_signalData_sint16;
							break;
					
						case SINT32:
					
							old_signalData_sint32 = *((sint32*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_sint32);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_sint32, new_signalData_sint32);
							if(old_signalData_sint32 != new_signalData_sint32)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(sint32*)(groupSignal->ComFGBuffer) = new_signalData_sint32;
							break;
					
						case SINT64:
					
							old_signalData_sint64 = *((sint64*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_sint64);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_sint64, new_signalData_sint64);
							if(old_signalData_sint64 != new_signalData_sint64)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(sint64*)(groupSignal->ComFGBuffer) = new_signalData_sint64;
							break;
					
						case SINT8:
					
							old_signalData_sint8 = *((sint8*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_sint8);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_sint8, new_signalData_sint8);
							if(old_signalData_sint8 != new_signalData_sint8)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(sint8*)(groupSignal->ComFGBuffer) = new_signalData_sint8;
							break;
					
						case UINT16:
					
							old_signalData_uint16 = *((uint16*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_uint16);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_uint16, new_signalData_uint16);
							if(old_signalData_uint16 != new_signalData_uint16)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(uint16*)(groupSignal->ComFGBuffer) = new_signalData_uint16;
							break;
					
						case UINT32:
					
							old_signalData_uint32 = *((uint32*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_uint32);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_uint32, new_signalData_uint32);
							if(old_signalData_uint32 != new_signalData_uint32)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(uint32*)(groupSignal->ComFGBuffer) = new_signalData_uint32;
							break;
					
						case UINT64:
					
							old_signalData_uint64 = *((uint64*)groupSignal->ComSignalDataPtr);
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_uint64);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_uint64, new_signalData_uint64);
							if(old_signalData_uint64 != new_signalData_uint64)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(uint64*)(groupSignal->ComFGBuffer) = new_signalData_uint64;
							break;
					
						case UINT8:
					
							old_signalData_uint8 = *((uint8*)groupSignal->ComSignalDataPtr);
							uint8 new_signalData_uint8;
							CopyGroupSignalFromSBtoAddress(signalGroup->ComHandleId, groupSignal->ComHandleId, &new_signalData_uint8);
							SignalGroupFilterResult = Com_ProcessTxSignalFilter_groupsignal(groupSignal, old_signalData_uint8, new_signalData_uint8);
							if(old_signalData_uint8 != new_signalData_uint8)
							{
								isSignalGroupChanged = 1;
							}
							else{}
							*(uint8*)(groupSignal->ComFGBuffer) = new_signalData_uint8;
							break;
					}
				}
				
	
				/*---------------------------------------------------Pack signal processed info in signal struct----------------------------------------------------*/		
					signalGroup->ComSignalGroupFilterResult = SignalGroupFilterResult;
	
					signalGroup->ComIsSignalGroupChanged = isSignalGroupChanged;
		
					/*----------------------------------------------------------------------------------------------------------------------------------------------------------
					[SWS_Com_00801] ⌈If the RTE updates a signal group by calling Com_SendSignalGroup, the AUTOSAR COM module shall set the update-bit of this signal
					group.⌋ (SRS_Com_02030)
					----------------------------------------------------------------------------------------------------------------------------------------------------------*/
					uint8 byteOffset = signalGroup->ComUpdateBitPosition /8;
					uint8 bitOffset = signalGroup->ComUpdateBitPosition %8 ;
					
					uint8* pduBufferPointer = (uint8*)Ipdu->ComIPduDataPtr;
					pduBufferPointer += byteOffset;
					uint8 updatebitMask = 1u << (bitOffset);
					*pduBufferPointer = *pduBufferPointer | updatebitMask;
				
					/*-----------------------------------------------IPDU Transmission Mode Selection---------------------------------------------------------*/
					Ipdu->ComTxIPdu->ComCurrentTransmissionSelection = com_pdu_transmissionsModeSelection(Ipdu);
					/*--------------------------Handle required number of transmissions of IPDU according to this signalGroup--------------------------------------*/
					if(com_pdu_transmissions_handle_signalGroup(Ipdu, signalGroup) == E_NOT_OK)
					{
						returnValue = COM_SERVICE_NOT_AVAILABLE;
					}
					else
					{
						returnValue = E_OK;
					}
			}
		}		
			
	}
	return returnValue;
}

 uint8 Com_ReceiveSignalGroup (Com_SignalGroupIdType SignalGroupId)
 {
     ComSignalGroup_type * SignalGroup= GET_SIGNALGROUP(GroupSignal->SignalGroupId);
     ComIPdu_type *Ipdu=GET_IPDU(SignalGroup->ComIPduHandleId);
	
	if(Ipdu->ComIPduDirection!=RECEIVE)
	{
		return E_NOT_OK;
	}
	else
	{
       if(Ipdu->ComIPduGroupRef!=NULL&&Ipdu->ComIPduGroupRef->IpduGroupFlag==STOPPED)
    {
        /*[SWS_Com_00461] ⌈The AUTOSAR COM module shall always copy the last known 
         data, or the ComSignalInitValue(s) if not yet written, of the I-PDU to the shadow buffer by a call to Com_ReceiveSignalGroup even if the I-PDU is stopped and COM_-
        SERVICE_NOT_AVAILABLE is returned*/
        CopySignalGroupfromSBtoFG( SignalGroupId);
        return COM_SERVICE_NOT_AVAILABLE;
    }
     else if((Ipdu->ComIPduGroupRef==NULL&&Com_GetStatus==COM_INIT)||(Ipdu->ComIPduGroupRef!=NULL&&Ipdu->ComIPduGroupRef->IpduGroupFlag==STARTED))
    {
        CopySignalGroupfromSBtoFG( SignalGroupId);
        return E_OK;
    }
	else
	{

	}

	}
    
 }
  Com_StatusType  Com_GetStatus(void)
  {
	return initStatus;
  }
 void Com_RxIndication (PduIdType RxPduId, const PduInfoType* PduInfoPtr)
 {
	if(Com_GetStatus()!= COM_INIT)
	{
		return;
	}
	else
	{
       ComIPdu_type *Ipdu=GET_IPDU(RxPduId);
	   ComIPdu_type *Ipdu_Rx=(ComIPdu_type *)PduInfoPtr->SduDataPtr;
	  
	if(Ipdu->ComIPduGroupRef!=NULL&&Ipdu->ComIPduGroupRef->IpduGroupFlag==STOPPED)
	{
		return;
	}
	else if((Ipdu->ComIPduGroupRef==NULL&&Com_GetStatus==COM_INIT)||(Ipdu->ComIPduGroupRef!=NULL&&Ipdu->ComIPduGroupRef->IpduGroupFlag==STARTED))
	{
		if (Ipdu->ComIPduCallout!=NULL)
		{
			Ipdu->ComIPduCallout();
		}
		else
		{

		}
		/*data sequence check*/
		uint8 excounter=check_Data_Sequence(Ipdu);
        uint8 recounter=check_Data_Sequence(Ipdu_Rx);
		if(recounter>((excounter+(Ipdu->ComIPduCounter->ComIPduCounterThreshold))%power(Ipdu->ComIPduCounter->ComIPduCounterSize)))
		{
			return;
		}
		else
		{
          excounter=(excounter+1)%power(Ipdu->ComIPduCounter->ComIPduCounterSize);
		  Std_ReturnType return1 =Com_writeCounterValueToPduBuffer(Ipdu, excounter);
		}
		
		
	}
	else
	{

	}
	for(uint16 signalid=0;Ipdu_Rx->ComIPduSignalRef[signalid]!=NULL;signalid++)
	{
	
			memcpy((uint8 *)Ipdu->ComIPduSignalRef[signalid]->ComBGBuffer,(uint8 *)Ipdu_Rx->ComIPduSignalRef[signalid]->ComSignalDataPtr,(Ipdu_Rx->ComIPduSignalRef[signalid]->ComBitSize)/8);
		

	}
	for(uint16 signalgroupid=0;Ipdu_Rx->ComIPduSignalGroupRef[signalgroupid]!=NULL;signalgroupid++)
	{
		
			memcpy((uint8 *)Ipdu->ComIPduSignalGroupRef[signalgroupid]->ComBGBuffer,(uint8 *)Ipdu_Rx->ComIPduSignalGroupRef[signalgroupid]->SignalGroupDataPtr,Ipdu_Rx->ComIPduSignalGroupRef[signalgroupid]->signalGroupSize);

	
		
	}
	return;
		
	}

 }
 uint8 Com_ReceiveSignal (Com_SignalIdType SignalId, void* SignalDataPtr)
 {
		
   if( SignalId>=0 && SignalId<=COM_MAX_SIGNAL)
   {
        ComSignal_type * Signal = GET_SIGNAL(SignalId);
	   if(Signal->containingIPDU->ComIPduDirection!=RECEIVE)
	   {
		  return E_NOT_OK;
	   }
	   else
	   {
		  if(Signal->containingIPDU->ComIPduGroupRef!=NULL&&Signal->containingIPDU->ComIPduGroupRef->IpduGroupFlag==STOPPED)
         {  
           return COM_SERVICE_NOT_AVAILABLE;
         }
          else if(Signal->containingIPDU->ComIPduGroupRef!=NULL&&Signal->containingIPDU->ComIPduGroupRef->IpduGroupFlag==STARTED)
        {    //CopySignalfromBGtoFG(SignalId);
            CopySignalFromFGtoAddress(SignalId,SignalDataPtr);
            return E_OK;
        }
		else if(Signal->containingIPDU->ComIPduGroupRef==NULL&&Com_GetStatus()==COM_INIT)
		{
			 CopySignalFromFGtoAddress(SignalId,SignalDataPtr);
             return E_OK;
		}
		else
		{

		}
       }
	}
    
     else if(SignalId >= COM_MIN_GROUPSIGNAL && SignalId <= COM_MAX_GROUPSIGNAL)
     {
         ComGroupSignal_type * GroupSignal = GET_GROUPSIGNAL(SignalId);
         ComSignalGroup_type * SignalGroup = GET_SIGNALGROUP(GroupSignal->SignalGroupId);
         ComIPdu_type *Ipdu=GET_IPDU(SignalGroup->ComIPduHandleId);

	     if(Ipdu->ComIPduDirection!=RECEIVE)
	     {
		    return E_NOT_OK;
	     }
	     else
	     {
            if(Ipdu->ComIPduGroupRef!=NULL&&Ipdu->ComIPduGroupRef->IpduGroupFlag==STOPPED)
            {
               return COM_SERVICE_NOT_AVAILABLE;
            }
            else if (Ipdu->ComIPduGroupRef==NULL&&Com_GetStatus()==COM_INIT)
			{
				CopyGroupSignalFromFGtoAddress(GroupSignal->SignalGroupId,SignalId,SignalDataPtr);
                return E_OK;
			}
			else if (Ipdu->ComIPduGroupRef!=NULL&&Ipdu->ComIPduGroupRef->IpduGroupFlag==STARTED)
			{
				CopyGroupSignalFromFGtoAddress(GroupSignal->SignalGroupId,SignalId,SignalDataPtr);
                return E_OK;
			}
			else
			{

			}
			
			
            
                
             
	     }
     
     }
     else
     {
	    return E_NOT_OK;
     }
 }



void Com_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
	if(Com_GetStatus() == COM_INIT)
	{
		ComIPdu_type* IPdu = GET_IPDU(TxPduId);
		ComIPduGroup_type* IPduGroup =  IPdu->ComIPduGroupRef;
		if(IPduGroup==NULL || IPduGroup->IpduGroupFlag == STARTED)
		{
			if(IPdu->ComIPduSignalProcessing == DEFERRED)
			{
				(IPdu->ComTxIPdu).ComIsIPduDeferred = 1;
			}
			else if(IPdu->ComIPduSignalProcessing == IMMEDIATE)
			{
				if(/*deadline monitor timeout*/)
				{
					for(uint16 signalID=0; (IPdu->ComIPduSignalRef[signalID] != NULL); signalID++)
					{
						if(IPdu->ComIPduSignalRef[signalID]->ComTimeoutNotification != NULL)
						{
							IPdu->ComIPduSignalRef[signalID]->ComTimeoutNotification();
						}
						else{}
					}
					for(uint16 signalGroupID=0; (IPdu->ComIPduSignalGroupRef[signalGroupID] != NULL); signalGroupID++)
					{
						if(IPdu->ComIPduSignalGroupRef[signalGroupID]->ComTimeoutNotification != NULL)
						{
							IPdu->ComIPduSignalGroupRef[signalGroupID]->ComTimeoutNotification();
						}
						else{}
					}
				}
				else if(result == E_OK)
				{
					for(uint16 signalID=0; (IPdu->ComIPduSignalRef[signalID] != NULL); signalID++)
					{
						if(IPdu->ComIPduSignalRef[signalID]->ComNotification != NULL)
						{
							IPdu->ComIPduSignalRef[signalID]->ComNotification();
						}
						else{}
					}
					for(uint16 signalGroupID=0; (IPdu->ComIPduSignalGroupRef[signalGroupID] != NULL); signalGroupID++)
					{
						if(IPdu->ComIPduSignalGroupRef[signalGroupID]->ComNotification != NULL)
						{
							IPdu->ComIPduSignalGroupRef[signalGroupID]->ComNotification();
						}
						else{}
					}
				}
				else if(result == E_NOT_OK)
				{
					if(IPduGroup != NULL && IPduGroup->IpduGroupFlag == STOPPED)
					{
						for(uint16 signalID=0; (IPdu->ComIPduSignalRef[signalID] != NULL); signalID++)
						{
							if(IPdu->ComIPduSignalRef[signalID]->ComErrorNotification != NULL)
							{
								IPdu->ComIPduSignalRef[signalID]->ComErrorNotification();
							}
							else{}
						}
						for(uint16 signalGroupID=0; (IPdu->ComIPduSignalGroupRef[signalGroupID] != NULL); signalGroupID++)
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
			}
			else{}
		}
		else{}
	}
	else{}
}

uint8 Com_InvalidateSignal(Com_SignalIdType SignalId)
{
	uint8 returnValue;
	if(Com_GetStatus() == COM_UNINIT)
	{
		returnValue = COM_SERVICE_NOT_AVAILABLE;
	}
	else
	{
		ComSignal_type* signal = GET_SIGNAL(SignalId);
		if(signal != NULL)
		{
			ComIPdu_type* IPdu = GET_IPDU(signal->ComIPduHandleId);
			if(IPdu == NULL || signal->ComSignalDataInvalidValue == NULL || (IPdu->ComIPduGroupRef)->IpduGroupFlag == STOPPED)
			{
				returnValue = COM_SERVICE_NOT_AVAILABLE;
			}
			else
			{
				if(Com_SendSignal(SignalId, signal->ComSignalDataInvalidValue) == E_OK)
				{
					returnValue = E_OK;
				}
				else
				{
					returnValue = COM_SERVICE_NOT_AVAILABLE;
				}
			}
		}
		else{}
	}
	return returnValue;		
}


uint8 Com_InvalidateSignalGroup (Com_SignalGroupIdType SignalGroupId)
{   uint8 flag=0;
	 ComSignalGroup_type * SignalGroup= GET_SIGNALGROUP(GroupSignal->SignalGroupId);
    ComIPdu_type *Ipdu=GET_IPDU(SignalGroup->ComIPduHandleId);
		if(Ipdu->ComIPduGroupRef==NULL)
	{
		if (Com_GetStatus()==COM_INIT)
		{
			 for(uint16 group_signalId=0;SignalGroup->ComGroupSignal[group_signalId]!=NULL;group_signalId++)
		{
          
			if(SignalGroup->ComGroupSignal[group_signalId]->ComSignalDataInvalidValue!=NULL)
			{
				flag=1;
				memcpy(SignalGroup->ComGroupSignal[group_signalId]->ComSignalDataPtr,SignalGroup->ComGroupSignal[group_signalId]->ComSignalDataInvalidValue,(SignalGroup->ComGroupSignal[group_signalId]->ComBitSize)/8);
			}
			else
			{

			}
		  
		  
		}
		if(flag==0)
		{
           return COM_SERVICE_NOT_AVAILABLE;
		}
		else
		{
			uint8 return_val=Com_SendSignalGroup(SignalGroupId);
			if(return_val==E_OK)
			{
			    return E_OK;
			}
			else
			{
				return E_NOT_OK;
			}
		}
		}
		else
		{

		}
		
     
	}
	
		else if(Ipdu->ComIPduGroupRef->IpduGroupFlag==STOPPED)
    {
        return COM_SERVICE_NOT_AVAILABLE;
    }
	else
	{
		 for(uint16 group_signalId=0;SignalGroup->ComGroupSignal[group_signalId]!=NULL;group_signalId++)
		{
          
			if(SignalGroup->ComGroupSignal[group_signalId]->ComSignalDataInvalidValue!=NULL)
			{
				flag=1;
				memcpy(SignalGroup->ComGroupSignal[group_signalId]->ComSignalDataPtr,SignalGroup->ComGroupSignal[group_signalId]->ComSignalDataInvalidValue,(SignalGroup->ComGroupSignal[group_signalId]->ComBitSize)/8);
			}
			else
			{

			}
		  
		  
		}
		if(flag==0)
		{
           return COM_SERVICE_NOT_AVAILABLE;
		}
		else
		{
			uint8 return_val=Com_SendSignalGroup(SignalGroupId);
			if(return_val==E_OK)
			{
			    return E_OK;
			}
			else
			{
				return E_NOT_OK;
			}
		}
	}
}


void Com_SwitchIpduTxMode (PduIdType PduId, boolean Mode)
{
	if(Com_GetStatus() == COM_INIT)
	{
		ComIPdu_type* IPdu = GET_IPDU(PduId);
		if(IPdu != NULL)
		{
			boolean oldTMS = IPdu->ComTxIPdu.ComCurrentTransmissionSelection;
			IPdu->ComTxIPdu.ComCurrentTransmissionSelection = Mode;
			/*--------------------------------------------------------------------------------------------------------------------------------------------------------
			When a call to Com_SendSignal or Com_SendSignalGroup results into a change of the transmission mode of a started I-PDU to the transmission mode PERIODIC
			or MIXED, then the AUTOSAR COM module shall start the new transmission cycle with a call to PduR_ComTransmit within the next main function at the latest.
			--------------------------------------------------------------------------------------------------------------------------------------------------------*/
			if((oldTMS == 0 && Mode == 1 && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode != DIRECT)
				||(oldTMS == 1 && Mode == 0 && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == DIRECT && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode != DIRECT))
			{
				 IPdu->ComTxIPdu.ComFirstPeriodicModeEntry = 1;
			}
			else if((oldTMS == 0 && Mode == 1 && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode != DIRECT && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode == DIRECT)
					||(oldTMS == 1 && Mode == 0 && IPdu->ComTxIPdu.ComTxModeTrue.ComTxMode.ComTxModeMode != DIRECT && IPdu->ComTxIPdu.ComTxModeFalse.ComTxMode.ComTxModeMode == DIRECT))
			{
				IPdu->ComTxIPdu.ComFirstDirectModeEntry = 1;
			}
			else{}
		}
		else{}
	}
	else{}
}
