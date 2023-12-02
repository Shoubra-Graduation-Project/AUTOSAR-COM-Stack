 
#ifndef _COM_TYPES_H_
#define _COM_TYPES_H_

#include "Std_Types.h"


/* Transmission modes for I-PDU */
typedef enum {
	DIRECT,
	MIXED,
	NONE,
    PERIODIC
} ComTxModeMode_enum;


/* Transfer Property for Signals */
typedef enum {
    PENDING,
    TRIGGERED,
    TRIGGERED_ON_CHANGE,
    TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
    TRIGGERED_WITHOUT_REPETITION
}ComTransferProperty_enum;



/* IPDU Directions */
typedef enum {
	RECEIVE,
	SEND
} ComIPduDirection_enum;



typedef enum {
	DEFERRED,
	IMMEDIATE
}ComIPduSignalProcessing_enum;


typedef enum {
    NORMAL,
    TP
}ComIPduType_enum;


typedef enum {
	BOOLEAN,
	UINT8,
	UINT16,
	UINT32,
	UINT64,
	SINT8,
	SINT16,
	SINT32,
	SINT64,
	FLOAT32,
	FLOAT64
} ComSignalType_enum;


typedef enum {
	CONFIRMATION,
	TRANSMIT,
	TRIGGER_TRANSMIT
}ComTxIPduClearUpdateBit_enum;

typedef enum{
	ALWAYS,
    MASKED_NEW_DIFFERS_MASKED_OLD,
    MASKED_NEW_DIFFERS_X, 
    MASKED_NEW_EQUALS_X,
    NEVER,
    NEW_IS_OUTSIDE,
    NEW_IS_WITHIN,
    ONE_EVERY_N
}ComFilterAlgorithm_enum;

/* This container contains the configuration parameters of the AUTOSAR COM module's transmission modes. */
typedef struct
{
	const ComTxModeMode_enum ComTxModeMode;
    
	/* 
	Defines the number of repetitions for the transmission mode DIRECT and
    the event driven part of transmission mode MIXED.	
	*/
	const uint8 ComTxModeNumberOfRepetitions;

    /* Defines the repetition period in s in
    case ComTxModeNumberOfRepetitions is configured greater than or
    equal to 1 and ComTxModeMode is configured to DIRECT or MIXED.
	*/
	const uint16 ComTxModeRepetitionPeriod;

	const uint32 ComTxModeTimeOffsetFactor;

	/*
	 Defines the repetition period in ms
	 in case ComTxModeMode is configured to PERIODIC or MIXED
	 */
	const uint16 ComTxModeTimePeriod;

} ComTxMode_struct;

/* If ComFilter evaluates to true */
typedef struct 
{
	const ComTxMode_struct ComTxMode;

} ComTxModeTrue_struct;

/* If  ComFilter evaluates to ComTxModeFalse */
typedef struct 
{
	const ComTxMode_struct ComTxMode;

} ComTxModeFalse_struct;


/* This container contains additional transmission related configuration parameters of the AUTOSAR COM module's I-PDUs. */
typedef struct 
{
	/* Minimum delay time between successive transmissions of the IPdu in s*/
	const uint32 ComMinimumDelayTime;

   /* Defines when the update-bits of contained in I-PDU will be cleared */
	const ComTxIPduClearUpdateBit_enum ComTxIPduClearUpdateBit;

	/* COM module fills not used areas of an I-PDU with this byte pattern*/
	const uint8 ComTxIPduUnusedAreasDefault;

	/* ComFilter evaluates to true */
	const ComTxModeTrue_struct ComTxModeTrue ;

	/* ComFilter evaluates to ComTxModeFalse */
	const ComTxModeFalse_struct ComTxModeFalse ;

} ComTxIPdu_struct;





#endif