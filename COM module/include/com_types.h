 
#ifndef _COM_TYPES_H_
#define _COM_TYPES_H_

#include "libraries/Std_Types.h"


/*****************************************************************
 *                        Enum Types                             *
 *****************************************************************/

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
    FLOAT32,
    FLOAT64,
    SINT16,
    SINT32,
    SINT64,
    SINT8,
    UINT16,
    UINT32,
    UINT64,
    UINT8,
    UINT8_DYN,
    UINT8_N
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


/**********************************************************************************************************
 *                                             Struct Types                                               *
 *********************************************************************************************************/                      



/*Contains the general configuration parameters of the Com module.*/
typedef struct{

	/* enables/disables the cancellation feature*/
	const boolean ComCancellationSupport;
    
	/*The error hook shall contain code to call the Det.*/
	const boolean ComConfigurationUseDet;
    
	/* Enables globally for the whole Com module the minimum delay time
       monitoring for cyclic and repeated transmissions
	*/
	const boolean ComEnableMDTForCyclicTransmission;

    /* Activate/Deactivate the signal group array access APIs */
	const boolean ComEnableSignalGroupArrayApi;

	const boolean ComRetryFailedTransmitRequests;
    
	/*Defines the maximum number of supported I-PDU groups*/
	const uint16 ComSupportedIPduGroups;
    
	/*Defines the header files for callback functions which shall be included by the COM module.*/
	void *const ComUserCbkHeaderFile;
    
	/*Activate/Deactivate the version information API (Com_GetVersionInfo).*/
	const boolean ComVersionInfoApi;

}ComGeneral_type;



/*This container contains the configuration parameters and sub containers of the COM module*/
typedef struct{
    
	/*Size of internal Com data in units of bytes (static memory allocation) */
	const uint64 ComDataMemSize;
    
	/*Maximum number of IPdus. */
	const uint64 ComMaxIPduCnt;

	/* IPDU definitions */
	const ComIPdu_type * ComIPdu;

	const ComIPduGroup_type ComIPduGroup;

	/* Signal definitions */
	const ComSignal_type *ComSignal;

	/* signal group Definition*/
	const ComSignalGroup_type *ComSignalGroup;

	const ComMainFunctionTx_type *ComMainFunctionRx;

	const ComMainFunctionRx_type *ComMainFunctionRx;

}ComConfig_type;



/* This container contains the configuration parameters of the AUTOSAR COM module's IPDUs */
typedef struct {
                   /*------------------------Parameters-------------------------*/
    /*This parameter defines the existence and the name of a callout function for the corresponding I-PDU*/
    boolean (* ComIPduCallout)  ( PduIdType PduId,const PduInfoType* PduInfoPtr);

    /* The numerical value used as the ID of the I-PDU */
    uint16 ComIPduHandleId ;

    /* Immediate or Deferred*/
    ComIPduSignalProcessing_type ComIPduSignalProcessing;

    /* sent or received */
    ComIPduDirection_type ComIPduDirection;

    /* If there is a trigger transmit callout defined for this I-PDU this parameter contains the name of the callout function */
    void (*ComIPduTriggerTransmitCallout) (void);

    /*Normal or TP*/
    ComIPduType_type ComIPduType;

    /* Defines for I-PDUs with ComIPduType NORMAL: If the underlying IF module supports cancellation of transmit requests.
       Defines for I-PDUs with ComIPduType TP: If the underlying TP-module supports RX and TX cancellation of ongoing requests.*/
    const boolean ComIPduCancellationSupport;

    /*Reference to the Com_MainFunctionRx/Com_MainFunctionTx this I-PDU belongs to.*/
    void (*ComIPduMainFunctionRef)(void);

                   /*------------------------Refrences-------------------------*/
    /*Reference to the I-PDU groups this I-PDU belongs to*/
    ComIPduGroup_type * ComIPduGroupRef;

    /*References to all signal groups contained in this I-Pdu*/
    ComSignalGroup_type * ComIPduSignalGroupRef;

    /* References to all signals contained in this I-PDU.*/
    ComSignal_type* ComIPduSignalRef;

    /*Reference to the "global" Pdu structure to allow harmonization of handle IDs in the COM-Stack.*/
    Pdu* ComPduIdRef;

                   /*------------------------Sub-containers-------------------------*/
    ComIPduCounter_type ComIPduCounter;

    ComIPduReplication_type ComIPduReplication;
} ComIPdu_type;



/* This container contains additional transmission related configuration parameters of the AUTOSAR COM module's I-PDUs. */
typedef struct 
{
	/* Minimum delay time between successive transmissions of the IPdu in s*/
	const float32 ComMinimumDelayTime;

   /* Defines when the update-bits of contained in I-PDU will be cleared */
	const ComTxIPduClearUpdateBit_type ComTxIPduClearUpdateBit;

	/* COM module fills not used areas of an I-PDU with this byte pattern*/
	const uint8 ComTxIPduUnusedAreasDefault;

	/* ComFilter evaluates to true */
	const ComTxModeTrue_type ComTxModeTrue ;

	/* ComFilter evaluates to ComTxModeFalse */
	const ComTxModeFalse_type ComTxModeFalse ;

} ComTxIPdu_type;



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


/* This container contains the configuration parameters of the COM module's transmission modes. */
typedef struct
{ 
	/*Direct, Mixed, None, Periodic*/
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

    /* period in seconds between the start of the I-PDU by Com_IpduGroupStart
	   and the first transmission request in case PERIODIC or MIXED Mode
	 */
	const float32 ComTxModeTimeOffsetFactor;

	/*
	 Defines the repetition period in seconds
	 in case ComTxModeMode is configured to PERIODIC or MIXED
	 */
	const float32 ComTxModeTimePeriod;

} ComTxMode_type;



typedef struct{
     uint8 ComIPduCounterSize;

     uint32 ComIPduCounterStartPosition;

     uint8 ComIPduCounterThreshold;

     void (*ComIPduCounterErrorNotification) (PduIdType, uint8, uint8);

}ComIPduCounter_type;



/*This optional container contains the information needed for each I-PDU replicated.*/
typedef struct{

   /*The number of identical I-PDUs needed for successful voting.*/
   const uint8 ComIPduReplicationQuorum;

   Pdu * ComIPduReplicaRef;

}ComIPduReplication_type;



typedef struct{
	/* The numerical value used as the ID of this I-PDU Group */
	const uint16 ComIPduGroupHandleId;
    
	/* References to all I-PDU groups that includes this I-PDU group. I */
	ComIPduGroup_type const * ComIPduGroupGroupRef;

}ComIPduGroup_type;



typedef struct {
    
	/* Starting position within the I-PDU */
	const uint32 ComBitPosition;
    
	/* Size in bits */
	const uint8 ComBitSize;

	/* Notify or Replace */
	ComDataInvalidAction_type ComDataInvalidAction;

	/* Name of Com_CbkTxErr callback function to be called. */
	void (*ComErrorNotification) (void);

	/* Length of the first deadline monitoring timeout period in s*/
	const float32 ComFirstTimeout;

	/* This ID identifies signals and signal groups in the COM APIs */
	const uint16 ComHandleId;

	/* This parameter defines that the respective signal's initial value shall be put
       into the respective PDU but there will not be any update of the value through the RTE.*/
	boolean ComInitialValueOnly;
	
	/* Name of Com_CbkInv callback function to be called */
	void (*ComInvalidNotification) (void);

	/*On sender side: Name of Com_CbkTxAck callback function to be called.
         On receiver side: Name of Com_CbkRxAck callback function to be called.*/
	void (*ComNotification) (void);

	/* Replace or Substitute or none*/
	ComRxDataTimeoutAction_type ComRxDataTimeoutAction;

	/* The data invalid value of the signal. */
	void *const ComSignalDataInvalidValue;

	/* Defines the endianness of the signal's network representation. */
	ComSignalEndianness_type ComSignalEndianness;

	/* Initial value for this signal. */
	void *const ComSignalInitValue;

	const uint32 ComSignalLength;

	ComSignalType_type ComSignalType;

	ComFilter_type ComFilter;

	/* The length of the deadline monitoring timeout period in seconds.  */
	const float32 ComTimeout;
    
	/*On sender side: Name of Com_CbkTxTOut callback function to be called.
       On receiver side: Name of Com_CbkRxTOut callback function to be called.*/
	void (*ComTimeoutNotification) (void);

	/* The signal substitution value will be used in case of a timeout */
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

	const uint32 ComUpdateBitPosition;
	
}ComSignalGroup_type;



typedef struct {

	const uint32 ComBitPosition;

	const uint8 ComBitSize;

	const uint16 ComHandleId;

	void *const  ComSignalDataInvalidValue;

	ComSignalEndianness_type ComSignalEndianness;

	void *const ComSignalInitValue;

	const uint32 ComSignalLength;

	ComSignalType_type ComSignalType;

	void *const ComTimeoutSubstitutionValue;

	ComTransferProperty_type ComTransferProperty;

}ComGroupSignal_type;


typedef struct{
	float32 ComMainRxTimeBase;
	//EcucPartition * ComMainRxPartitionRef;
}ComMainFunctionRx_type;



typedef struct{
	float32 ComMainTxTimeBase;
	//EcucPartition * ComMainTxPartitionRef;
	void (*ComPreparationNotification) (void);
}ComMainFunctionTx_type;



typedef struct{
	ComFilterAlgorithm_type ComFilterAlgorithm;
	sint64 ComFilterMask;
	sint64 ComFilterMax;
	sint64 ComFilterMin;
	uint32 ComFilterOffset;
	uint32 ComFilterPeriod;
	sint64 ComFilterX;
}ComFilter_type;

#endif

