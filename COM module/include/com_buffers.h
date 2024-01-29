#ifndef INCLUDE_COM_BUFFERS_H_
#define INCLUDE_COM_BUFFERS_H_


void Com_CopyShadowBufferToIPDU (const Com_SignalGroupIdType signalGroupId);
void Com_CopyPduToShadowBuffer(const Com_SignalGroupIdType signalGroupId) ;
void CopyGroupSignalFromSBtoAddress(const ComSignalGroup_type SignalGroup_id,void *dataAddress);
void CopySignalFromFGtoAddress(const ComSignalGroup_type Signal_id,void *dataAddress);
#endif