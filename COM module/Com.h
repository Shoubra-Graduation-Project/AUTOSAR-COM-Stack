#ifndef _COM_H_
#define _COM_H_
/*------------------------------------------Includes Start-------------------------------------------------------------------*/
#include "Std_Types.h"
/*------------------------------------------Includes End---------------------------------------------------------------------*/


/*------------------------------------------Macro Declarations Start---------------------------------------------------------*/
/* The service is currently not available */
#define  COM_SERVICE_NOT_AVAILABLE 0x80

/* Successful execution of this function is currently not possible */
#define COM_BUSY 0x81
/*------------------------------------------Macro Declarations End-----------------------------------------------------------*/


/*------------------------------------------Macro Function Declarations Start------------------------------------------------*/
/*------------------------------------------Macro Function Declarations End--------------------------------------------------*/


/*------------------------------------------ComStack Type Declarations Start-------------------------------------------------*/
/* The COM module's signal object identifier. */
typedef uint16 Com_SignalIdType;

/* The COM module's signal group object identifier */
typedef uint16 Com_SignalGroupIdType;

/* The COM I-PDU module's group object identifier. */
typedef uint16 Com_IpduGroupIdType;
/*------------------------------------------ComStack Type Declarations End---------------------------------------------------*/


/*------------------------------------------Data Type Declarations Start-----------------------------------------------------*/
/*
 This is a status value returned by the API service Com_GetStatus().
*/
typedef enum 
{
 COM_UNINIT = 0x00,
 COM_INIT	
}Com_StatusType;
/*------------------------------------------Data Type Declarations End-------------------------------------------------------*/


/*------------------------------------------Software Interfaces Declarations Start-------------------------------------------*/
/**   /brief    Com Stack Initialization Function  /details  This service initializes internal and external interfaces and variables of the AUTOSAR COM module layer for the further 
			processing. After calling this function the inter-ECU communication is still disabled.  /param	config: Pointer to the AUTOSAR COM module's configuration data.  /return	None */void Com_Init (const Com_ConfigType* config);/**   /brief    Com Stack De-Initialization Function  /details  This service stops the inter-ECU communication. All started I-PDU groups are stopped and have to be started again, if needed, 
			after Com_Init is called. By a call to Com_DeInit the AUTOSAR COM module is put into an not initialized state.  /param	None  /return	None */void Com_DeInit (void);/**   /brief    Enable Reception Deadline Monitor  /details  Enables the reception deadline monitoring for the I-PDUs within the given IPDU group.  /param	IpduGroupId: Id of I-PDU group where reception DM shall be enabled.  /return	None */void Com_EnableReceptionDM (Com_IpduGroupIdType IpduGroupId);/**   /brief    Disable Reception Deadline Monitor  /details  Disables the reception deadline monitoring for the I-PDUs within the given IPDU group.  /param	IpduGroupId: Id of I-PDU group where reception DM shall be disabled.  /return	None */void Com_DisableReceptionDM (Com_IpduGroupIdType IpduGroupId);/**   /brief    Send Signal  /details  The service Com_SendSignal updates the signal object identified by SignalId with the signal referenced by the SignalDataPtr parameter.  /param	SignalId: Id of signal to be sent.
			SignalDataPtr: Reference to the signal data to be transmitted.  /return	uint8: E_OK: service has been accepted.
					COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development error).
					COM_BUSY: in case the TP-Buffer is locked for large data types handling. */uint8 Com_SendSignal (Com_SignalIdType SignalId,const void* SignalDataPtr);


/**   /brief    Receive Signal  /details  Com_ReceiveSignal copies the data of the signal identified by SignalId to the location specified by SignalDataPtr.  /param	SignalId Id of signal to be received.  /return	uint8: E_OK: service has been accepted.
					COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development error).
					COM_BUSY: in case the TP-Buffer is locked for large data types handling. */uint8 Com_ReceiveSignal (Com_SignalIdType SignalId, void* SignalDataPtr);


/**   /brief    Send Signal Group  /details  The service Com_SendSignalGroup copies the content of the associated shadow buffer to the associated I-PDU.  /param	SignalGroupId: Id of signal group to be sent.  /return	uint8: E_OK: service has been accepted.
					COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development error).
					COM_BUSY: in case the TP-Buffer is locked for large data types handling. */uint8 Com_SendSignalGroup (Com_SignalGroupIdType SignalGroupId);


/**   /brief    Receive Signal Group  /details  The service Com_ReceiveSignalGroup copies the received signal group from the I-PDU to the shadow buffer.  /param	SignalGroupId: Id of signal group to be received.  /return	uint8: E_OK: service has been accepted.
					COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group was stopped (or service failed due to development error).
					COM_BUSY: in case the TP-Buffer is locked for large data types handling. */uint8 Com_ReceiveSignalGroup (Com_SignalGroupIdType SignalGroupId);


/**   /brief    Invalidate Signal  /details  The service Com_InvalidateSignal invalidates the signal with the given SignalId by setting its value to its configured ComSignalDataInvalidValue.  /param	SignalId: Id of signal to be invalidated.  /return	uint8: E_OK: service has been accepted.
					COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group is stopped, no ComSignalDataInvalidValue is configured for the given signalId or
					service fails due to development error.
					COM_BUSY: in case the TP-Buffer is locked. */uint8 Com_InvalidateSignal (Com_SignalIdType SignalId);

/**   /brief    Invalidate Signal Group  /details  The service Com_InvalidateSignalGroup invalidates all group signals of the signal group with the given SignalGroupId by setting their values 			to their configured Com SignalDataInvalidValues.  /param	SignalGroupId: Id of signal group to be invalidated.  /return	uint8: E_OK: service has been accepted.
					COM_SERVICE_NOT_AVAILABLE: corresponding I-PDU group is stopped, no ComSignalDataInvalidValue is configured for the given signalId or
					service fails due to development error.
					COM_BUSY: in case the TP-Buffer is locked. */uint8 Com_InvalidateSignalGroup (Com_SignalGroupIdType SignalGroupId);


/**   /brief    Receive Indication  /details  Indication of a received PDU from a lower layer communication interface module.  /param	RxPduId: ID of the received PDU.
			PduInfoPtr: Contains the length (SduLength) of the received PDU, a pointer to a buffer (SduDataPtr) containing the PDU, and the MetaData 
			related to this PDU.  /return	None */void Com_RxIndication (PduIdType RxPduId, const PduInfoType* PduInfoPtr);/**   /brief    Transmission confirmation  /details  The lower layer communication interface module confirms the transmission of a PDU, or the failure to transmit a PDU.  /param	TxPduId: ID of the PDU that has been transmitted.
			result: E_OK: The PDU was transmitted. 
					E_NOT_OK: Transmission of the PDU failed.  /return	None */void Com_TxConfirmation (PduIdType TxPduId, Std_ReturnType result);/**   /brief      Start of Reception  /details    This function is called at the start of receiving an N-SDU. The N-SDU might be fragmented into multiple N-PDUs (FF with one or more following CFs) 
			  or might consist of a single N-PDU (SF). The service shall provide the currently available maximum buffer size when invoked with TpSduLength equal to 0.  /param-in	  id: Identification of the I-PDU.
			  info: Pointer to a PduInfoType structure containing the payload data (without protocol information) and payload length of the first frame or single frame
				  of a transport protocol I-PDU reception, and the MetaData related to this PDU. If neither first/single frame data nor MetaData are available, this
				  parameter is set to NULL_PTR.
			  TpSdu: Length Total length of the N-SDU to be received.  /param-out  bufferSizePtr: Available receive buffer in the receiving module. This parameter will be used to compute the Block Size (BS) in the transport 							 protocol module.  /return	  BufReqReturnType:  BUFREQ_OK: Connection has been accepted. bufferSizePtr indicates the available receive buffer; reception is continued. 
											If no buffer of the requested size is available, a receive buffer size of 0 shall be indicated by bufferSizePtr.
			  BUFREQ_E_NOT_OK: Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
			  BUFREQ_E_OVFL: No buffer of the required length can be provided; reception is aborted. bufferSizePtr remains unchanged. */BufReq_ReturnType Com_StartOfReception (PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr);/**   /brief      Copy Reception Data  /details    This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer. Each call to this function provides 			  the next part of the I-PDU data. The size of the remaining buffer is written to the position indicated by bufferSizePtr.  /param-in	  id: Identification of the received I-PDU.
			  info: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength). An SduLength of 0 can be used to query the 
			  current amount of available buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR.  /param-out  bufferSizePtr:     Available receive buffer after data has been copied.  /return	  BufReqReturnType:  BUFREQ_OK: Data copied successfully.
								 BUFREQ_E_NOT_OK: Data was not copied because an error occurred. */BufReq_ReturnType Com_CopyRxData (PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);/**   /brief      Copy Reception Data  /details    This function is called to provide the received data of an I-PDU segment (N-PDU) to the upper layer. Each call to this function provides 			  the next part of the I-PDU data. The size of the remaining buffer is written to the position indicated by bufferSizePtr.  /param-in	  id: Identification of the received I-PDU.
			  info: Provides the source buffer (SduDataPtr) and the number of bytes to be copied (SduLength). An SduLength of 0 can be used to query the 
			  current amount of available buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR.  /param-out  bufferSizePtr:     Available receive buffer after data has been copied.  /return	  BufReqReturnType:  BUFREQ_OK: Data copied successfully.
								 BUFREQ_E_NOT_OK: Data was not copied because an error occurred. */BufReq_ReturnType Com_CopyRxData (PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);

/**   /brief      Copy Transmission Data  /details    This function is called to acquire the transmit data of an I-PDU segment (N-PDU). Each call to this function provides the next part of 
			  the I-PDU data unless retry->Tp DataState is TP_DATARETRY.
  /param-in	  id: Identification of the transmitted I-PDU.
			  info: Provides the destination buffer (SduDataPtr) and the number of bytes to be copied (SduLength).			  retry: This parameter is used to acknowledge transmitted data or to retransmit data after transmission problems.  /param-out  availableDataPtr: Indicates the remaining number of bytes that are available in the upper layer module's Tx buffer.  /return	  BufReqReturnType: BUFREQ_OK:       Data has been copied to the transmit buffer completely as requested.
								BUFREQ_E_BUSY:   Request could not be fulfilled, because the required amount of Tx data is not available. The lower layer 
											     module may retry this call later on. No data has been copied.
								BUFREQ_E_NOT_OK: Data has not been copied. Request failed. */BufReq_ReturnType Com_CopyTxData (PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr);

/*------------------------------------------Software Interfaces Declarations End---------------------------------------------*/

#endif