
#ifndef _COM_HELPINGFUNCTIONS_H_
#define _COM_HELPINGFUNCTIONS_H_


#include "Std_Types.h"
#include "Com_types.h"


 


uint8 Com_ProcessTxSignalFilter(ComSignal_type* signalStruct, uint64 oldData, uint64 newData);
uint8 power(unsigned n);
uint8 Com_ProcessTxSignalFilter_float(ComSignal_type* signalStruct, float64 oldData, float64 newData);
boolean check_Data_Sequence(ComIPdu_type *Ipdu,ComIPdu_type *Ipdu_Rx);
#endif
