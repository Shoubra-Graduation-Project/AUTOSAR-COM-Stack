#include "include/Com_buffers.h"
#include "include/Com_Types.h"
#include "include/Com_Cfg.h"
#include "include/ComMacros.h"
#include <string.h>
#include <cstddef>
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

void Com_CopyShadowBufferToIPDU (const Com_SignalGroupId signalGroupId)
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
void CopyGroupSignalFromSBtoAddress(const ComSignalGroup_type SignalGroup_id,void *dataAddress)
{
    const ComSignalGroup_Type * SignalGroup=GET_SIGNALGROUP(signalGroupId);
    
}
void CopySignalFromFGtoAddress(const ComSignalGroup_type Signal_id,void *dataAddress)
{
    const ComSignal_Type * Signal=GET_SIGNAL(signalGroupId);
    memcpy((uint8 *)dataAddress,(uint8 *)Signal->ComFGBuffer,Signal->ComSignalLength);
}
