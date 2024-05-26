#ifndef _COM_HELPINGFUNCTIONS_H_
#define _COM_HELPINGFUNCTIONS_H_


#include "../libraries/Std_Types.h"
#include "../include/com_types.h"

void delay(uint32 seconds);
uint8 power(unsigned n);
boolean Com_ProcessTxSignalFilter(ComSignal_type* signalStruct, uint64 oldData, uint64 newData);
boolean Com_ProcessTxSignalFilter_float(ComSignal_type* signalStruct, float64 oldData, float64 newData);
boolean Com_ProcessTxSignalFilter_float_groupsignal(ComGroupSignal_type* signalStruct, float64 oldData, float64 newData);
boolean Com_ProcessTxSignalFilter_groupsignal(ComGroupSignal_type* signalStruct, uint64 oldData, uint64 newData);
uint8 com_pdu_transmissions_handle_signal(ComIPdu_type* IPdu, ComSignal_type* signal);
uint8 com_pdu_transmissions_handle_signalGroup(ComIPdu_type* IPdu, ComSignalGroup_type* signalGroup);
boolean com_pdu_transmissionsModeSelection(ComIPdu_type* IPdu);
void com_packSignalsToPdu(ComIPdu_type* IPdu);
uint8 Com_writeCounterValueToPduBuffer(ComIPdu_type *IPdu, uint8 counterdata);
uint8 check_Data_Sequence(ComIPdu_type *Ipdu);
#endif
