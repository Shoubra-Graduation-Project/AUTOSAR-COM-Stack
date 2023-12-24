#ifndef _COM_H_
#define _COM_H_

#include "libraries/Std_Types.h"
#include "com_types.h"

/* The service is currently not available */
#define  COM_SERVICE_NOT_AVAILABLE 0x80

/* Successful execution of this function is currently not possible */
#define COM_BUSY 0x81

/* The COM module's signal object identifier. */
typedef uint16 Com_SignalIdType;

/* The COM module's signal group object identifier */
typedef uint16 Com_SignalGroupIdType;

/* The COM I-PDU module's group object identifier. */
typedef uint16 Com_IpduGroupIdType;

/*
 This is a status value returned by the API service Com_GetStatus().
*/
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
	const ComIPduGroup_type ComIPduGroup;

	 /* Contains the configuration parameters of the COM module's signals. */
	const ComSignal_type *ComSignal;

	/* Contains the configuration parameters of the COM module's signal groups. */
	const ComSignalGroup_type *ComSignalGroup;

}Com_ConfigType;


void Com_CopyToShadowBuffer();
void Com_CopyIpduToShadowBuffer();
void Com_CopyFromShadowBuffer();

#endif