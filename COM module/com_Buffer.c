#include "include/Com_buffers.h"
#include "include/Com_Types.h"
#include "include/Com_Cfg.h"
#include "include/ComMacros.h"
#include "include/Com.h"
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





/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_WriteSignalDataToPduBuffer                                                             
 * 
 *    Parameters (in): signalId, signalData
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Write signal data to Ipdu buffer
 * 
 *********************************************************************************/

void Com_WriteSignalDataToPduBuffer(const uint16 signalId, const void *signalData)
{
	uint32 BitPosition;
	uint8 data;
	uint8 mask;
	uint8 pduMask;
	uint8 signalMask;
	uint8 *pduBufferBytes = NULL;
	uint8 *pduBeforChange = NULL;
	uint8 *dataBytes = NULL;
	uint8 signalLength;
	uint8 BitOffsetInByte;
	uint8 pduStartByte;
	uint8 i;

    // Get signal
	const ComSignal_type * Signal =  GET_Signal(signalId);

	// Get PDU
	const ComIPdu_type *IPdu = GET_IPdu(Signal->ComIPduHandleId);

    // Set pduBuffer to the ComIPduDataPtr pointer
	void * const PduBuffer = IPdu->ComIPduDataPtr;


    
	BitPosition = Signal->ComBitPosition;
	BitOffsetInByte = BitPosition % 8;
	pduStartByte = BitPosition / 8;
	pduBufferBytes = (uint8 *)PduBuffer;
	dataBytes = (uint8 *) signalData;
	signalLength = Signal->ComBitSize/8;
	pduBeforChange = pduBufferBytes;

	pduBufferBytes += pduStartByte;

	for(i = 0; i<=signalLength; i++)
	{
	    pduMask = 255;
	    signalMask = 255;
        if( i == 0)
        {
            pduMask = mask >> (8 - BitOffsetInByte);
            signalMask = signalMask >> BitOffsetInByte;
            *pduBufferBytes = (* pduBufferBytes) & pduMask;
            data = (* dataBytes) & signalMask;
            data = data << BitOffsetInByte;
            *pduBufferBytes = (* pduBufferBytes) | data;
            pduBufferBytes ++;
        }
        else if(i==signalLength)
        {
            pduMask = mask << BitOffsetInByte;
            signalMask = signalMask << (8 - BitOffsetInByte);
            *pduBufferBytes = (* pduBufferBytes) & pduMask;
            data = (* dataBytes) & signalMask;
            data = data >> (8 - BitOffsetInByte);
            *pduBufferBytes = (* pduBufferBytes) | data;
        }
        else
        {
            pduMask = mask << BitOffsetInByte;
            signalMask = signalMask << (8 - BitOffsetInByte);
            *pduBufferBytes = (* pduBufferBytes) & pduMask;
            data = (* dataBytes) & signalMask;
            data = data >> (8 - BitOffsetInByte);
            *pduBufferBytes = (* pduBufferBytes) | data;

            dataBytes++;

            pduMask = 255;
            signalMask = 255;
            pduMask = mask >> (8 - BitOffsetInByte);
            signalMask = signalMask >> BitOffsetInByte;
            *pduBufferBytes = (* pduBufferBytes) & pduMask;
            data = (* dataBytes) & signalMask;
            data = data << BitOffsetInByte;
            *pduBufferBytes = (* pduBufferBytes) | data;
            pduBufferBytes ++;

        }

	}





}


/***********************************************************************************
 *                                                                                 *
 *    Service Name: Com_WriteSignalDataToPdu                                                             
 * 
 *    Parameters (in): signalId, signalData
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Write signal data to Ipdu 
 * 
 *********************************************************************************/
void Com_WriteSignalDataToPdu(const Com_SignalIdType signalId, const void *signalData) 
{
	// Get Signal
	const ComSignal_type *Signal     = GET_Signal(signalId);

    // Get Ipdu
	const ComIPdu_type   *IPdu       = GET_IPdu(Signal->ComIPduHandleId);

	// Get data
	Com_WriteSignalDataToPduBuffer(signalId, signalData);
}