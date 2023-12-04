#ifndef _COM_H_
#define _COM_H_
/*------------------------------------------Includes Start------------------------------------------------*/
#include "Std_Types.h"

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

#endif