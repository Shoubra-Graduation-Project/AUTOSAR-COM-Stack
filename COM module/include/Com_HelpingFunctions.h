
#ifndef _COM_HELPINGFUNCTIONS_H_
#define _COM_HELPINGFUNCTIONS_H_
<<<<<<< HEAD

#include "Std_Types.h"
#include "Com_types.h"
=======
 boolean is_com_initiated(const Com_ConfigType* config);
 
>>>>>>> 3a7ff5f1a09d8fe976ab09676323eed6525b6de1

uint8 Com_ProcessTxSignalFilter(ComSignal_type* signalStruct, uint64 oldData, uint64 newData);

uint8 Com_ProcessTxSignalFilter_float(ComSignal_type* signalStruct, float64 oldData, float64 newData);

#endif
