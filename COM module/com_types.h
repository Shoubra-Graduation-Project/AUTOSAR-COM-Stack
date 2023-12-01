 
#ifndef _COM_TYPES_H_
#define _COM_TYPES_H_

#include "Std_Types.h"

typedef uint16 Com_SignalIdType;
typedef uint16 Com_SignalGroupIdType;

/* Transmission modes for I-PDU */
typedef enum {
	DIRECT,
	MIXED,
	NONE,
    PERIODIC
} ComTxModeMode;

/* Transfer Property for Signals */
typedef enum {
    PENDING,
    TRIGGERED,
    TRIGGERED_ON_CHANGE,
    TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
    TRIGGERED_WITHOUT_REPETITION
}ComTransferProperty;


/* IPDU Directions */
typedef enum {
	RECEIVE,
	SEND
} ComIPduDirection;


typedef enum {
	DEFERRED,
	IMMEDIATE
}ComIPduSignalProcessing;

typedef enum {
    NORMAL,
    TP
}ComIPduType;


typedef enum {
	BOOLEAN,
	FLOAT32,
	FLOAT64,
	UINT8,
	UINT16,
	UINT32,
	UINT8_N,
	UINT8_DYN,
	SINT8,
	SINT16,
	SINT32,
	SINT64,
	UINT64
} ComSignalType;


#endif