 
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


typedef enum{
	NOTIFY,
    REPLACE 
}ComDataInvalidAction_type;

typedef enum{
	NONE,
	REPLACE,
	SUBSTITUTE
}ComRxDataTimeoutAction_type;

typedef enum{
 	BIG_ENDIAN,
    LITTLE_ENDIAN,
    OPAQUE 

}ComSignalEndianness_type;

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
	
	
	 /*This parameter defines the existence and the name of a callout function for the corresponding I-PDU*/
	 boolean (* ComIPduCallout)  ( PduIdType PduId,const PduInfoType* PduInfoPtr);

	 /*Reference to the Com_MainFunctionRx/Com_MainFunctionTx this I-PDU
	 belongs to.*/
	 void (*ComIPduMainFunctionRef)(void);

	  /*Reference to the I-PDU groups this I-PDU belongs to*/
	 ComIPduGroup* ComIPduGroupRef;

	 /*References to all signal groups contained in this I-Pdu*/
	 ComSignalGroup* ComIPduSignalGroupRef;

	 /* References to all signals contained in this I-PDU.*/
	 ComSignal_type* ComIPduSignalRef;

	 /*Reference to the "global" Pdu structure to allow harmonization of handle
	 IDs in the COM-Stack.*/
	 Pdu* ComPduIdRef;


   
} ComIPdu_type;


typedef struct {
    
	/* Starting position within the I-PDU */
	const uint32 ComBitPosition;
    
	/* Size in bits */
	const uint8 ComBitSize;

    /* Notify or Replace */
    ComDataInvalidAction_type ComDataInvalidAction;

    /* Length of the first deadline monitoring timeout period in s*/
	const float32 ComFirstTimeout;

    /* This ID identifies signals and signal groups in the COM APIs */
	const uint16 ComHandleId;

    /* This parameter defines that the respective signal's initial value shall be put
       into the respective PDU but there will not be any update of the value through the RTE.
	*/
	boolean ComInitialValueOnly;
  
    /* Replace or Substitute or none*/
	ComRxDataTimeoutAction_type ComRxDataTimeoutAction;

    /* The data invalid value of the signal. */
	void *const ComSignalDataInvalidValue;

    /* The endianness of the signal's network representation */
	ComRxDataTimeoutAction_type ComRxDataTimeoutAction;


	void *const ComSignalInitValue;

	ComSignalType_type ComSignalType;


	/* The length of the deadline monitoring timeout period in seconds.  */
	const float32 ComTimeout;


    void *const ComTimeoutSubstitutionValue;

	ComTransferProperty_type ComTransferProperty;


	const uint32 ComUpdateBitPosition;

}ComSignal_type;


typedef struct{

    /* Replace or Notify */
    ComDataInvalidAction_type ComDataInvalidAction;

	void (*ComErrorNotification) (void);

	const float32 ComFirstTimeout;

	const uint16 ComHandleId;

	boolean ComInitialValueOnly;

	void (*ComInvalidNotification) (void);

	void (*ComNotification) (void);

	ComRxDataTimeoutAction_type ComRxDataTimeoutAction;

	const float32 ComTimeout;

	void (*ComTimeoutNotification) (void);

	ComTransferProperty_type ComTransferProperty;

	const uint32 ComUpdateBitPositionl
	

}ComSignalGroup_type;

/*This container contains the configuration parameters and sub containers of the COM module.*/

typedef struct{

	

	/* IPDU definitions */
	const ComIPdu_type * ComIPdu;

	/* Signal definitions */
	const ComSignal_type *ComSignal;

	/* signal group Definition*/
	const ComSignalGroup_Type *ComSignalGroup;




}ComConfig_type;


#endif