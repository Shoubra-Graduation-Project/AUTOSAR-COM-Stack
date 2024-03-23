#ifndef INCLUDE_COM_BUFFERS_H_
#define INCLUDE_COM_BUFFERS_H_


void Com_CopyShadowBufferToIPDU (const Com_SignalGroupIdType signalGroupId);
void Com_CopyPduToShadowBuffer(const Com_SignalGroupIdType signalGroupId) ;
void CopyGroupSignalFromSBtoAddress(const ComSignalGroup_type SignalGroup_id,void *dataAddress);
void CopySignalFromFGtoAddress(const Com_SignalIdType Signal_id,void *dataAddress);
void CopySignalGroupfromBGtoSB(Com_SignalGroupIdType SignalGroupId);
void CopySignalfromBGtoFG(Com_SignalIdType Signal_id);
void CopySignalGroupfromSBtoFG(Com_SignalGroupIdType SignalGroupId);
void CopyGroupSignalFromFGtoAddress(const ComSignalGroup_type SignalGroup_id, const ComGroupSignal_type GroupSignal_id, void *dataAddress);

#endif
