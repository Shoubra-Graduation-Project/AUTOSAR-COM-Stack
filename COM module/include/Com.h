
#ifndef _COM_H_
#define _COM_H_
/*------------------------------------------Includes Start-------------------------------------------------------------------*/
#include "libraries/Std_Types.h"
#include "com_types.h"
#include "ComStack_Types.h"
/*------------------------------------------Includes End---------------------------------------------------------------------*/


/*------------------------------------------Macro Declarations Start---------------------------------------------------------*/
/* The service is currently not available */
#define  COM_SERVICE_NOT_AVAILABLE 0x80

/* Successful execution of this function is currently not possible */
#define COM_BUSY 0x81
/*------------------------------------------Macro Declarations End-----------------------------------------------------------*/



/*------------------------------------------ComStack Type Declarations Start-------------------------------------------------*/
/* The COM module's signal object identifier. */
typedef uint16 Com_SignalIdType;

/* The COM module's signal group object identifier */
typedef uint16 Com_SignalGroupIdType;

/* The COM I-PDU module's group object identifier. */
typedef uint16 Com_IpduGroupIdType;
/*------------------------------------------ComStack Type Declarations End---------------------------------------------------*/


/*------------------------------------------Data Type Declarations Start-----------------------------------------------------*/
/*This is a status value returned by the API service Com_GetStatus()*/
typedef enum 
{
 COM_UNINIT = 0x00,
 COM_INIT	
}Com_StatusType;

typedef struct{
    
	/*Size of internal Com data in units of bytes (static memory allocation) */
	const uint64 ComDataMemSize;
    
	/*Maximum number of IPdus. */
	const uint64 ComMaxIPduCnt;

	 /* Contains the configuration parameters of the  COM module's IPDUs. */
	const ComIPdu_type * ComIPdu;

    /* Contains the configuration parameters of the COM module's IPDU groups. */
	const ComIPduGroup_type *ComIPduGroup;

	 /* Contains the configuration parameters of the COM module's signals. */
	const ComSignal_type *ComSignal;

	/* Contains the configuration parameters of the COM module's signal groups. */
	const ComSignalGroup_type *ComSignalGroup;

}Com_ConfigType;
/*------------------------------------------Data Type Declarations End-------------------------------------------------------*/


/*------------------------------------------Software Interfaces Declarations Start-------------------------------------------*/

void Com_Init (const Com_ConfigType* config);



void Com_DeInit (void);

void Com_EnableReceptionDM (Com_IpduGroupIdType IpduGroupId);

void Com_DisableReceptionDM (Com_IpduGroupIdType IpduGroupId);

uint8 Com_SendSignal (Com_SignalIdType SignalId,const void* SignalDataPtr);

uint8 Com_ReceiveSignal (Com_SignalIdType SignalId, void* SignalDataPtr);

uint8 Com_SendSignalGroup (Com_SignalGroupIdType SignalGroupId);

uint8 Com_ReceiveSignalGroup (Com_SignalGroupIdType SignalGroupId);

uint8 Com_InvalidateSignal (Com_SignalIdType SignalId);

uint8 Com_InvalidateSignalGroup (Com_SignalGroupIdType SignalGroupId);

void Com_RxIndication (PduIdType RxPduId, const PduInfoType* PduInfoPtr);

void Com_TxConfirmation (PduIdType TxPduId, Std_ReturnType result);

BufReq_ReturnType Com_StartOfReception (PduIdType id, const PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr);

BufReq_ReturnType Com_CopyRxData (PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);

BufReq_ReturnType Com_CopyRxData (PduIdType id, const PduInfoType* info, PduLengthType* bufferSizePtr);

BufReq_ReturnType Com_CopyTxData (PduIdType id, const PduInfoType* info, const RetryInfoType* retry, PduLengthType* availableDataPtr);



#endif
