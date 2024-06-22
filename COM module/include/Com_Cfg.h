#ifndef _COM_CFG_H_
#define _COM_CFG_H_

#include "Com.h"

#define COM_NUM_OF_IPDU 4
#define COM_NUM_OF_SIGNAL 8

#define COM_SIGNAL_INIT_VALUE (uint8)0xFF
#define COM_TIMEOUT_SUBSTITUTION_VALUE (uint8)0xAA

extern  Com_ConfigType ComConfig;

#endif