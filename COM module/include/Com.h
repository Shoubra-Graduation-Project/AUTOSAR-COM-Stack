
#ifndef _COM_H_
#define _COM_H_

#include "../libraries/Std_Types.h"
#include "../include/com_types.h"
#include "../include/ComStack_Types.h"




/* The service is currently not available */
#define  COM_SERVICE_NOT_AVAILABLE 0x80

/* Successful execution of this function is currently not possible */
#define COM_BUSY 0x81



/* The COM module's signal object identifier. */
typedef uint16 Com_SignalIdType;

/* The COM module's signal group object identifier */
typedef uint16 Com_SignalGroupIdType;
typedef uint16 Com_GroupSignalIdType;

/* The COM I-PDU module's group object identifier. */
typedef uint16 Com_IpduGroupIdType;



/*This is a status value returned by the API service Com_GetStatus()*/
typedef enum 
{
   // The AUTOSAR COM module is initialized and usable.
   COM_UNINIT = 0x00,

   /*The AUTOSAR COM module is not initialized or not usable. This
   shall be the default value after reset. This status shall have the
   value 0. */
   COM_INIT	
}Com_StatusType;

typedef struct{
    
	/*Size of internal Com data in units of bytes (static memory allocation) */
	const uint64 ComDataMemSize;
    
	/*Maximum number of IPdus. */
	const uint64 ComMaxIPduCnt;

	 /* Contains the configuration parameters of the  COM module's IPDUs. */
	ComIPdu_type ** ComIPdu;

    /* Contains the configuration parameters of the COM module's IPDU groups. */
	ComIPduGroup_type **ComIPduGroup;

	 /* Contains the configuration parameters of the COM module's signals. */
	ComSignal_type **ComSignal;

	/* Contains the configuration parameters of the COM module's signal groups. */
	ComSignalGroup_type **ComSignalGroup;
	
	/* Contains the configuration parameters of the COM module's signal groups. */
	ComGroupSignal_type **ComGroupSignal;
	
   

}Com_ConfigType;





void Com_Init (const Com_ConfigType* config);

Com_StatusType  Com_GetStatus(void);

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





#endif
