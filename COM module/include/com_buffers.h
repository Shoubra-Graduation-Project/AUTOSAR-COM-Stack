#ifndef INCLUDE_COM_BUFFERS_H_
#define INCLUDE_COM_BUFFERS_H_

#include "../../Common/Std_Types.h"
#include "./com.h"

void Com_CopyShadowBufferToIPDU (const uint16 signalGroupId);
void Com_CopyPduToShadowBuffer(const uint16 signalGroupId) ;
void CopyGroupSignalFromSBtoAddress(const Com_SignalGroupIdType SignalGroup_id, const Com_GroupSignalIdType GroupSignal_id, const void *dataAddress);
void CopySignalFromFGtoAddress(const uint16 Signal_id,void *dataAddress);
void CopySignalGroupfromBGtoSB(uint16 SignalGroupId);
void CopySignalfromBGtoFG(uint16 Signal_id);
void CopySignalGroupfromSBtoFG(uint16 SignalGroupId);
void CopyGroupSignalFromFGtoAddress(const uint16 SignalGroup_id, const uint16 GroupSignal_id, void *dataAddress);
void Com_WriteGroupSignalDataToPdu(const uint16 signalId, const void *signalData);
void Com_WriteSignalDataToPdu(const uint16 signalId, const void *signalData);
#endif
