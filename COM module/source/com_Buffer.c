/*****************************************************************
 * 
 *                             Includes
 * 
*****************************************************************/

#include "../include/Com_buffers.h"
#include "../include/Com_Types.h"
#include "../include/Com_Cfg.h"
#include "../include/ComMacros.h"
//#include "../include/Com.h"


#include <string.h>
#include <cstddef>




/***********************************************************************
 *                                                                       
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
 *************************************************************************/

/*
void Com_CopyShadowBufferToIPDU (const Com_SignalGroupIdType signalGroupId)
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
*/
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

/*
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
*/

/***********************************************************************
 *                                                                       
 *    Service Name: CopyGroupSignalFromSBtoAddress                                                             
 * 
 *    Parameters (in): SignalGroup_id, GroupSignal_id, dataAddress
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description: 
 * 
 *************************************************************************/


void CopyGroupSignalFromSBtoAddress(const Com_SignalGroupIdType SignalGroup_id, const Com_GroupSignalIdType GroupSignal_id, const void *dataAddress)
{
    // Get signal
    const ComSignalGroup_type * SignalGroup;
    const ComGroupSignal_type * GroupSignal;
    uint8* shadowBuffer;
	  uint8* data=(uint8*)dataAddress;
    uint32 byteSteps;
    uint32 signalLength;
		uint8 groupSignalByteOffsetInSB = 0;
  	int i;
    // Get signal
    SignalGroup = GET_SIGNALGROUP(SignalGroup_id);

    // Get signal group
    GroupSignal = GET_GROUPSIGNALCNFG(GroupSignal_id);


    shadowBuffer = (uint8*)SignalGroup->ComShadowBuffer;

		for(i=0; i<GroupSignal->ComHandleId; i++)
		{
			groupSignalByteOffsetInSB += (SignalGroup->ComGroupSignal[i]->ComBitSize)/8;
		}
    shadowBuffer += groupSignalByteOffsetInSB;
    signalLength = (GroupSignal->ComBitSize)/8;
		
		for(i=0; i<signalLength; i++)
		{
			*data = *shadowBuffer;
			shadowBuffer++;
			data++;
		}
    
}

void Com_WriteSignalDataToShadowBuffer(const Com_SignalGroupIdType SignalGroup_id, const Com_GroupSignalIdType GroupSignal_id, const void *dataPtr)
{
	const ComSignalGroup_type * SignalGroup;
    const ComGroupSignal_type * GroupSignal;
    uint8* shadowBuffer;
    uint32 byteSteps;
    uint32 signalLength;
		uint8 groupSignalByteOffsetInSB = 0;
		uint8 * dataPointer = (uint8*)dataPtr;
  	int i;
    // Get signal
    SignalGroup = GET_SIGNALGROUP(SignalGroup_id);

    // Get signal group
    GroupSignal = GET_GROUPSIGNALCNFG(GroupSignal_id);


    shadowBuffer = (uint8*)SignalGroup->ComShadowBuffer;

		for(i=0; i<GroupSignal->ComHandleId; i++)
		{
			groupSignalByteOffsetInSB += (SignalGroup->ComGroupSignal[i]->ComBitSize)/8;
		}
    shadowBuffer += groupSignalByteOffsetInSB;
    signalLength = (GroupSignal->ComBitSize)/8;
		
		for(i=0; i<signalLength; i++)
		{
			*(uint8*)shadowBuffer = *dataPointer;
			shadowBuffer++;
			dataPointer++;
		}
    
}
void CopyGroupSignalFromFGtoAddress(const Com_SignalGroupIdType SignalGroup_id, const Com_GroupSignalIdType GroupSignal_id, void *dataAddress)
{
		uint8 i;
		uint8 GroupSignalOffsetInBuffer = 0;
    const ComSignalGroup_type * SignalGroup;
    const ComGroupSignal_type * GroupSignal;
	  uint8* FGBuffer, *address;
    uint8 signalLength;
   // Get signal
   SignalGroup = GET_SIGNALGROUP(SignalGroup_id);

   // Get signal group
   GroupSignal = GET_GROUPSIGNAL(SignalGroup_id, GroupSignal_id);

    for(i=0; i<GroupSignal_id; i++)
		{
			ComGroupSignal_type *gs = GET_GROUPSIGNAL(SignalGroup_id, i);
			GroupSignalOffsetInBuffer += (gs->ComBitSize)/8;
		}
		
    FGBuffer = (uint8*)SignalGroup->ComFGBuffer;
		address = (uint8*)dataAddress;
		FGBuffer += GroupSignalOffsetInBuffer;
    signalLength = (GroupSignal->ComBitSize)/8;
		
    for( i = 0; i<signalLength; i++)
    {
			*FGBuffer = *(uint8*)address;
			FGBuffer++;
			address++;
    }
}
/***********************************************************************
 *                                                                       
 *    Service Name: CopySignalFromFGtoAddress                                                             
 * 
 *    Parameters (in): Signal_id, dataAddress
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  
 * 
 *************************************************************************/
void CopySignalFromFGtoAddress(const Com_SignalIdType Signal_id,void *dataAddress)
{ 
    // Get Signal
    const ComSignal_type * Signal = GET_SIGNAL(Signal_id);

    // memcpy(dest, src, size)
    memcpy((uint8*)dataAddress,(uint8*)Signal->ComFGBuffer,(Signal->ComBitSize)/8);
}
/***********************************************************************
 *                                                                       
 *    Service Name: CopySignalGroupfromBGtoSB                                                             
 * 
 *    Parameters (in): SignalGroupId
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  
 * 
 *************************************************************************/
void CopySignalGroupfromBGtoSB(Com_SignalGroupIdType SignalGroupId)
{
    // Get signal group
    const ComSignalGroup_type * SignalGroup = GET_SIGNALGROUP(SignalGroupId);

    // memcpy(dest, src, size)
    memcpy((uint8*)SignalGroup->ComShadowBuffer,(uint8*)SignalGroup->ComBGBuffer,SignalGroup->signalGroupSize);

}
void CopySignalGroupfromSBtoFG(Com_SignalGroupIdType SignalGroupId)
{
  const ComSignalGroup_type * SignalGroup = GET_SIGNALGROUP(SignalGroupId);
  memcpy((uint8*)SignalGroup->ComFGBuffer,(uint8*)SignalGroup->ComShadowBuffer,SignalGroup->signalGroupSize);
}
/***********************************************************************
 *                                                                       
 *    Service Name: CopySignalfromBGtoFG                                                             
 * 
 *    Parameters (in): Signal_id
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description: 
 * 
 *************************************************************************/
void CopySignalfromBGtoFG(Com_SignalIdType Signal_id)
{
    // Get Signal
    const ComSignal_type * Signal = GET_SIGNAL(Signal_id);
    
    // memcpy(dest, src, size)
    memcpy((uint8*)Signal->ComFGBuffer, (uint8*)Signal->ComBGBuffer,(Signal->ComBitSize)/8);
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
	const ComSignal_type * Signal =  GET_SIGNAL(signalId);

	// Get PDU
	const ComIPdu_type *IPdu = GET_IPDU(Signal->ComIPduHandleId);

    // Set pduBuffer to the ComIPduDataPtr pointer
	void  const * PduBuffer = IPdu->ComIPduDataPtr;


    
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
	// Get data
	Com_WriteSignalDataToPduBuffer(signalId, signalData);
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

void Com_WriteGroupSignalDataToPduBuffer(const uint16 groupSignalId, const void *signalData)
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
	const ComGroupSignal_type * GroupSignal =  GET_GROUPSIGNALCNFG(groupSignalId);

	// Get PDU
	const ComIPdu_type *IPdu = GET_IPDU(GroupSignal->ComIPduHandleId);

    // Set pduBuffer to the ComIPduDataPtr pointer
	void  const * PduBuffer = IPdu->ComIPduDataPtr;


    
	BitPosition = GroupSignal->ComBitPosition;
	BitOffsetInByte = BitPosition % 8;
	pduStartByte = BitPosition / 8;
	pduBufferBytes = (uint8 *)PduBuffer;
	dataBytes = (uint8 *) signalData;
	signalLength = GroupSignal->ComBitSize/8;
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
void Com_WriteGroupSignalDataToPdu(const Com_SignalIdType signalId, const void *signalData) 
{
	
	// Get data
	Com_WriteSignalDataToPduBuffer(signalId, signalData);
}
