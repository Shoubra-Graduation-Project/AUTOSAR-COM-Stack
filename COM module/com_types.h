 
#ifndef _COM_TYPES_H_
#define _COM_TYPES_H_

#include "Std_Types.h"


/* Transmission modes for I-PDU */
typedef enum {
	DIRECT,
	MIXED,
	NONE,
    PERIODIC
} ComTxModeMode_type;


/* Transfer Property for Signals */
typedef enum {
    PENDING,
    TRIGGERED,
    TRIGGERED_ON_CHANGE,
    TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
    TRIGGERED_WITHOUT_REPETITION
}ComTransferProperty_type;



/* IPDU Directions */
typedef enum {
	RECEIVE,
	SEND
} ComIPduDirection_type;



typedef enum {
	DEFERRED,
	IMMEDIATE
}ComIPduSignalProcessing_type;


typedef enum {
    NORMAL,
    TP
}ComIPduType_type;


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
} ComSignalType_type;


typedef enum {
	CONFIRMATION,
	TRANSMIT,
	TRIGGER_TRANSMIT
}ComTxIPduClearUpdateBit_type;

typedef enum{
	ALWAYS,
    MASKED_NEW_DIFFERS_MASKED_OLD,
    MASKED_NEW_DIFFERS_X, 
    MASKED_NEW_EQUALS_X,
    NEVER,
    NEW_IS_OUTSIDE,
    NEW_IS_WITHIN,
    ONE_EVERY_N
}ComFilterAlgorithm_type;

/* This container contains the configuration parameters of the AUTOSAR COM module's transmission modes. */
typedef struct
{
	const ComTxModeMode_type ComTxModeMode;
    
	/* 
	Defines the number of repetitions for the transmission mode DIRECT and
    the event driven part of transmission mode MIXED.	
	*/
	const uint8 ComTxModeNumberOfRepetitions;

    /* Defines the repetition period in s in
    case ComTxModeNumberOfRepetitions is configured greater than or
    equal to 1 and ComTxModeMode is configured to DIRECT or MIXED.
	*/
	const float32 ComTxModeRepetitionPeriod;

	const uint32 ComTxModeTimeOffsetFactor;

	/*
	 Defines the repetition period in ms
	 in case ComTxModeMode is configured to PERIODIC or MIXED
	 */
	const uint16 ComTxModeTimePeriod;

} ComTxMode_type;

/* If ComFilter evaluates to true */
typedef struct 
{
	const ComTxMode_type ComTxMode;

} ComTxModeTrue_type;

/* If  ComFilter evaluates to ComTxModeFalse */
typedef struct 
{
	const ComTxMode_type ComTxMode;

} ComTxModeFalse_type;


/* This container contains additional transmission related configuration parameters of the AUTOSAR COM module's I-PDUs. */
typedef struct 
{
	/* Minimum delay time between successive transmissions of the IPdu in s*/
	const uint32 ComMinimumDelayTime;

   /* Defines when the update-bits of contained in I-PDU will be cleared */
	const ComTxIPduClearUpdateBit_type ComTxIPduClearUpdateBit;

	/* COM module fills not used areas of an I-PDU with this byte pattern*/
	const uint8 ComTxIPduUnusedAreasDefault;

	/* ComFilter evaluates to true */
	const ComTxModeTrue_type ComTxModeTrue ;

	/* ComFilter evaluates to ComTxModeFalse */
	const ComTxModeFalse_type ComTxModeFalse ;

} ComTxIPdu_type;


/* This container contains the configuration parameters of the AUTOSAR COM module's IPDUs */
typedef struct {
	
	/*
	   Defines for I-PDUs with ComIPduType NORMAL: If the underlying IF module supports cancellation of transmit requests.
       Defines for I-PDUs with ComIPduType TP: If the underlying TP-module supports RX and TX cancellation of ongoing requests.
    */
    const boolean ComIPduCancellationSupport;

    /* sent or received */
    ComIPduDirection_type ComIPduDirection;

    /* The numerical value used as the ID of the I-PDU */
    uint16 ComIPduHandleId ;

	/* Immediate or Deferred*/
	ComIPduSignalProcessing_type ComIPduSignalProcessing;

    /*Normal or TP*/
	ComIPduType_type ComIPduType;
	
	/*aya*/ 
	 /*This parameter defines the existence and the name of a callout function for
	 the corresponding I-PDU*/
	 boolean (* ComIPduCallout)  (
	 PduIdType PduId,
	 const PduInfoType* PduInfoPtr
	 );
	 /*Reference to the Com_MainFunctionRx/Com_MainFunctionTx this I-PDU
	 belongs to.*/
	 void (*ComIPduMainFunctionRef)(void);
	  /*Reference to the I-PDU groups this I-PDU belongs to*/
	 ComIPduGroup* ComIPduGroupRef;
	 /*References to all signal groups contained in this I-Pdu*/
	 ComSignalGroup* ComIPduSignalGroupRef;
	 /* References to all signals contained in this I-PDU.*/
	 ComSignal* ComIPduSignalRef;
	 /*Reference to the "global" Pdu structure to allow harmonization of handle
	 IDs in the COM-Stack.*/
	 Pdu* ComPduIdRef;


   
} ComIPdu_type;


typedef struct {

	const uint64 ComBitPosition;

	




}ComSignal_type;


#endif