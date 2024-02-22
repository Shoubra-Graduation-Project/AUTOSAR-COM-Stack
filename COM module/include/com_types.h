
#ifndef _COM_TYPES_H_
#define _COM_TYPES_H_

#include "libraries/Std_Types.h"

typedef enum
{
	STOPPED,
	STARTED
}state_type;
/***************************************************************************************************
Name: Com_ReturnType

Type: EcucEnumerationParamDef

Description: Possible return values of com APIs
****************************************************************************************************/
typedef enum {
	E_OK,
	COM_SERVICE_NOT_AVAILABLE,
	COM_BUSY
} Com_ReturnType;

/***************************************************************************************************
Name: ComTxModeMode

Type: EcucEnumerationParamDef

Description: Transmission modes for I-PDU
****************************************************************************************************/
typedef enum {
	DIRECT,
	MIXED,
	NONE,
    PERIODIC
} ComTxModeMode_type;

/***************************************************************************************************
Name: ComTransferProperty

Type: EcucEnumerationParamDef

Description: Transfer Property for Signals
****************************************************************************************************/
typedef enum {
    PENDING,
    TRIGGERED,
    TRIGGERED_ON_CHANGE,
    TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,
    TRIGGERED_WITHOUT_REPETITION
}ComTransferProperty_type;


/***************************************************************************************************
Name: ComIPduDirection

Type: EcucEnumerationParamDef

Description: IPDU Directions
****************************************************************************************************/
typedef enum {
	RECEIVE,
	SEND
} ComIPduDirection_type;


/***************************************************************************************************
Name: ComIPduSignalProcessing

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
typedef enum {
	DEFERRED,
	IMMEDIATE
}ComIPduSignalProcessing_type;

/***************************************************************************************************
Name: ComIPduType

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
typedef enum {
    NORMAL,
    TP
}ComIPduType_type;

/***************************************************************************************************
Name: ComSignalType

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
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

/***************************************************************************************************
Name: ComTxIPduClearUpdateBit

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
typedef enum {
	CONFIRMATION,
	TRANSMIT,
	TRIGGER_TRANSMIT
}ComTxIPduClearUpdateBit_type;

/***************************************************************************************************
Name: ComFilterAlgorithm

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
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

/***************************************************************************************************
Name: ComDataInvalidAction

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
typedef enum{
	NOTIFY,
    REPLACE 
}ComDataInvalidAction_type;

/***************************************************************************************************
Name: ComRxDataTimeoutAction

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
typedef enum{
	NONE,
	REPLACE,
	SUBSTITUTE
}ComRxDataTimeoutAction_type;

/***************************************************************************************************
Name: ComSignalEndianness

Type: EcucEnumerationParamDef

Description: 
****************************************************************************************************/
typedef enum{
 	BIG_ENDIAN,
    LITTLE_ENDIAN,
    OPAQUE 

}ComSignalEndianness_type;


/********************************************************************************************
Name: ComGeneral

Type: Structure

Description: Contains the general configuration parameters of the Com module.
********************************************************************************************/
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

	boolean comPeriodicTimeFired;

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
	const float32 ComMinimumDelayTime;

   /* Defines when the update-bits of contained in I-PDU will be cleared */
	const ComTxIPduClearUpdateBit_type ComTxIPduClearUpdateBit;

	/* COM module fills not used areas of an I-PDU with this byte pattern*/
	const uint8 ComTxIPduUnusedAreasDefault;

	/* ComFilter evaluates to true */
	const ComTxModeTrue_type ComTxModeTrue ;

	/* ComFilter evaluates to ComTxModeFalse */
	const ComTxModeFalse_type ComTxModeFalse ;

	/*Not in SWS*/
	uint32 ComNumberOfTransmissions;

	boolean ComCurrentTransmissionSelection;

	boolean ComIsIPduDeferred;

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

    /* 
	   If there is a trigger transmit callout defined for this I-PDU this parameter
       contains the name of the callout function 
	*/
	void (*ComIPduTriggerTransmitCallout) (void);

    /*Normal or TP*/
	ComIPduType_type ComIPduType;
	
	
	 /*This parameter defines the existence and the name of a callout function for the corresponding I-PDU*/
	 boolean (* ComIPduCallout)  ( PduIdType PduId,const PduInfoType* PduInfoPtr);

	 /*Reference to the Com_MainFunctionRx/Com_MainFunctionTx this I-PDU
	 belongs to.*/
	 void (*ComIPduMainFunctionRef)(void);

	  /*Reference to the I-PDU groups this I-PDU belongs to*/
	 ComIPduGroup_type * ComIPduGroupRef;

	 /*References to all signal groups contained in this I-Pdu*/
	 ComSignalGroup_type ** ComIPduSignalGroupRef;

	 /* References to all signals contained in this I-PDU.*/
	 ComSignal_type** ComIPduSignalRef;

	 /*Reference to the "global" Pdu structure to allow harmonization of handle
	 IDs in the COM-Stack.*/
	 Pdu* ComPduIdRef;

	 ComTxIPdu_type ComTxIPdu;
    
	/*Pointer to IPDU data ---->  Not in SWS*/
    void const * ComIPduDataPtr;

	/*Size of IPDU ---->  Not in SWS*/
	const uint8 ComIPduLength;
    
	/* ----> Not in SWS */
	boolean ReceptionDMEnabled;
   
} ComIPdu_type;


/********************************************************************************************
Name: ComIPduGroup

Type: Structure

Description: 
********************************************************************************************/
typedef struct{
	/* The numerical value used as the ID of this I-PDU Group */
	const uint16 ComIPduGroupHandleId;

	state_type IpduGroupFlag;
    
	/* References to all I-PDU groups that includes this I-PDU group. I */
	ComIPduGroup_type const * ComIPduGroupGroupRef;

    /* 
	 Check if Reception deadline monitoring for this IpduGruop is enabled or not
     ---> Not in SWS
	*/ 
	boolean RxDeadlineMonitoringEnabled;

    /*-------> not in SWS */
	ComIPdu_type *IPDU;

	/*
	   Number of IPDUs within this group
	   ----> Not in SWS
	*/
    uint16 numIPdus;
	

}ComIPduGroup_type;




/********************************************************************************************
Name: ComSignal_type

Type: Structure

Description: 
********************************************************************************************/
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


	/* The length of the deadline monitoring timeout period in seconds.  */
	const float32 ComTimeout;
    
	/*On sender side: Name of Com_CbkTxTOut callback function to be called.
       On receiver side: Name of Com_CbkRxTOut callback function to be called.*/
	void (*ComTimeoutNotification) (void);

	/* The signal substitution value will be used in case of a timeout */
	void *const ComTimeoutSubstitutionValue;

	ComTransferProperty_type ComTransferProperty;

	const uint32 ComUpdateBitPosition;

    
	/*Pointer to signal data ----> Not in SWS*/
	void * const ComSignalDataPtr;

    /* Pointer to foreground buffer       -->not in SWS */
    void * ComFGBuffer; 

	/* Pointer to background buffer       -->not in SWS */ 
	void * ComBGBuffer; 

	/* I-PDU that contain this signal ---------> Not in SWS*/
	const uint16 ComIPduHandleId;
    
	/* -----> Not in SWS*/
	float32 DeadlineMonitoringTimer;

    // ---> Not is SWS
	const boolean IsGroupSignal;

	boolean ComIsSignalChanged;

	boolean ComSignalFilterResult;

    ComIPdu_type *containingIPDU;

	boolean ComSignalUpdated;

}ComSignal_type;




/********************************************************************************************
Name: ComSignalGroup

Type: Structure

Description: 
********************************************************************************************/
typedef struct{

    /* Replace or Notify */
    ComDataInvalidAction_type ComDataInvalidAction;

    /* Only valid on sender side: Name of Com_CbkTxErr callback function to be called.*/
	void (*ComErrorNotification) (void);

    /*Defines the length of the first deadline monitoring timeout period in seconds*/
	const float32 ComFirstTimeout;
    
	/*The numerical value used as the ID.*/
	const Com_SignalGroupIdType ComHandleId;

    /*
	  This parameter defines that the respective signal's initial value shall be put
      into the respective PDU but there will not be any update of the value through the RTE.
	*/
	boolean ComInitialValueOnly;

    /*Only valid on receiver side: Name of Com_CbkInv callback function to be called.*/
	void (*ComInvalidNotification) (void);

   /*
     On sender side: Name of Com_CbkTxAck callback function to be called.
     On receiver side: Name of Com_CbkRxAck callback function to be called.
	 */
	void (*ComNotification) (void);

    /*None - Replace - Substitute*/
	ComRxDataTimeoutAction_type ComRxDataTimeoutAction;

    /* Defines the length of the deadline monitoring timeout period in seconds. */
	const float32 ComTimeout;

    /*
	  On sender side: Name of Com_CbkTxTOut callback function to be called.
      On receiver side: Name of Com_CbkRxTOut callback function to be called.
	*/
	void (*ComTimeoutNotification) (void);

     /*SizeOfsignalGroup in bytes ------>not in SWS*/
    const uint32 signalGroupSize;

	ComTransferProperty_type ComTransferProperty;
    
	/*Bit position of update-bit inside I-PDU.*/
	const uint32 ComUpdateBitPosition;

	/* Group signals included in this signal group  -------> Not in SWS*/
	ComGroupSignal_type **ComGroupSignal;
    
	/* Identify shadow buffer -------> Not in SWS*/    
	 void * ComShadowBuffer;

	/* Identify BackGround buffer -------> Not in SWS*/  
	 void *ComFGBuffer;

	/* Identify BackGround buffer -------> Not in SWS*/
    	 void * ComBGBuffer; 

	/* I-PDU that contain this signal group ---------> Not in SWS*/
	 uint16 ComIPduHandleId;

	 void * SignalGroupDataPtr;
	
	boolean ComIsSignalGroupChanged;

	boolean ComSignalGroupFilterResult;

	/* -----> Not in SWS*/
	float32 DeadlineMonitoringTimer;

}ComSignalGroup_type;




/********************************************************************************************
Name: ComGroupSignal

Type: Structure

Description: 
********************************************************************************************/
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

/* I-PDU that contain this signal ---------> Not in SWS*/
	const uint16 ComIPduHandleId;
    
	void const * ComSignalDataPtr;

	const Com_SignalGroupIdType SignalGroupId;
    
	// ----> Not is SWS
	const boolean IsGroupSignal ;

 
}ComGroupSignal_type;

typedef struct{

   /*Name of Com_CbkCounterErr callback function to be called. */
   void (*ComIPduCounterErrorNotification) (void);
   
   /*Size of PDU Counter expressed in bits*/
   const uint8 ComIPduCounterSize;

   /*Position of PDU counter expressed in bits from start position of data content*/
   const uint32 ComIPduCounterStartPosition;
   
   /* Threshold value of I-PDU counter algorithm*/
   const uint8 ComIPduCounterThreshold;

}ComIPduCounter_type;

/*This optional container contains the information needed for each I-PDU replicated.*/
typedef struct{

   /*The number of identical I-PDUs needed for successful voting.*/
   const uint8 ComIPduReplicationQuorum;

   Pdu * ComIPduReplicaRef;


}ComIPduReplication_type;

/*This container contains the configuration parameters and sub containers of the COM module.*/

typedef struct{
	ComFilterAlgorithm_type ComFilterAlgorithm;
	sint64 ComFilterMask;
	sint64 ComFilterMax;
	sint64 ComFilterMin;
	uint32 ComFilterOffset;
	uint32 ComFilterPeriod;
	uint32 ComFilterOccurrence;
	sint64 ComFilterX;
}ComFilter_type;


#endif


