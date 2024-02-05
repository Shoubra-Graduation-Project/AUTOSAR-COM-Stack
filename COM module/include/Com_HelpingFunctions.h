
#ifndef _COM_HELPINGFUNCTIONS_H_
#define _COM_HELPINGFUNCTIONS_H_
 boolean is_com_initiated(const Com_ConfigType* config);
 

uint8 Com_ProcessTxSignalFilter(ComSignal_type* signalStruct, uint64 oldData, uint64 newData);

uint8 Com_ProcessTxSignalFilter_float(ComSignal_type* signalStruct, float64 oldData, float64 newData);

#endif
