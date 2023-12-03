#ifndef _COM_H_
#define _COM_H_#define  COM_SERVICE_NOT_AVAILABLE 0x80#define COM_BUSY 0x81
typedef unsigned short Com_SignalIdTypetypedef unsigned short Com_SignalGroupIdTypetypedef unsigned short Com_IpduGroupIdType
/*
 This is a status value returned by the API service Com_GetStatus().
*/
typedef enum 
{
 COM_UNINIT = 0x00,
 COM_INIT	
}Com_StatusType;