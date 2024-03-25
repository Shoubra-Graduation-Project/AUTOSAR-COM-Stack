
#include "../../Common/Std_Types.h"
/*#include "include/Com_Types.h"
#include "include/Com_Cfg.h"*/


const ComGroupSignal_Type ComGSignals[]=
{
 {    // Group signal 1
       .ComBitPosition= 0,
       .ComHandleId= 0 ,
       .ComBitSize= 16,
       .ComSignalGroupId=0,
       .ComSignalType = UINT16,
       .ComTransferProperty = TRIGGERED_ON_CHANGE,
       .ComSignalDataPtr=ComGSignalBuffer_0
   },
   //group signal 2
   {
    .ComHandleId=1,
    .ComBitPosition=0,
    .ComSignalGroupId=0,
    .ComBitSize=8,
    .ComSignalType=UINT8,
    .ComTransferProperty=TRIGGERED_ON_CHANGE,
    .ComSignalDataPtr=ComGSignalBuffer_1
   }
   
};


const ComSignal_type ComSignal[] = 
{

    {	//signal1
        .ComBitPosition= 17,
        .ComUpdateBitPosition= 25,
        .ComHandleId= 1 ,
        .ComBitSize= 8,
        .ComSignalType = BOOLEAN,
        .ComTransferProperty = TRIGGERED_ON_CHANGE,
        .ComIPduHandleId=0,
		.ComNotification=NULL,
		.ComSignalDataPtr = ComSignalBuffer_1
    },
	{	//signal2
		.ComBitPosition= 17,
		.ComUpdateBitPosition= 25,
		.ComHandleId= 1 ,
		.ComBitSize= 8,
		.ComSignalType = BOOLEAN,
		.ComTransferProperty = TRIGGERED_ON_CHANGE,
		.ComIPduHandleId=0,
		.ComNotification=NULL,
		.ComSignalDataPtr = ComSignalBuffer_1
	},
	{	//signal3
		.ComBitPosition= 17,
		.ComUpdateBitPosition= 25,
		.ComHandleId= 1 ,
		.ComBitSize= 8,
		.ComSignalType = BOOLEAN,
		.ComTransferProperty = TRIGGERED_ON_CHANGE,
		.ComIPduHandleId=0,
		.ComNotification=NULL,
		.ComSignalDataPtr = ComSignalBuffer_1
	},
    {	//signal4
        .ComBitPosition= 26,
        .ComUpdateBitPosition= 58 ,
        .ComHandleId= 2 ,
        .ComBitSize= 32,
        .ComSignalType = UINT32,
        .ComTransferProperty = TRIGGERED_ON_CHANGE,
        .ComIPduHandleId=0,
		.ComNotification=NULL,
		.ComSignalDataPtr = ComSignalBuffer_2
    }

};
/* group signal with different imagination :) */
//const ComGroupSignal_Type * const groupS0[]={
//       &ComSignal[0],
//       &ComSignal[1]
//};
//
//const ComGroupSignal_Type *const groupS1[]={
//  &ComSignal[2],
//  &ComSignal[3],
//  &ComSignal[4]
//};

/* Group signals of signal groups */
const ComGroupSignal_Type * const ComSignalGroup_GroupSignalsRefs1[]={
     &ComGSignals[ 0 ],
     &ComGSignals[ 1 ],
     NULL
};

const ComGroupSignal_Type * const ComSignalGroup_GroupSignalsRefs2[]={
     &ComGSignals[ 2 ],
     &ComGSignals[ 3 ],
     NULL
};

/* IPdu signal lists. */
const ComSignal_type * const ComIPduSignalRefs_Can_Message_1[] = {
    &ComSignal[ CanDB_Signal_32_21_BE_Tester ],
    &ComSignal[ CanDB_Signal_1_4_LE_Tester ],
    &ComSignal[ CanDB_Signal_45_12_LE_Tester ],
	NULL
};

const ComSignal_type * const ComIPduSignalRefs_Can_Message_2[] = {
    &ComSignal[ CanDB_Signal_32_21_BE ],
    &ComSignal[ CanDB_Signal_29_12_BE_Tester],
    &ComSignal[ CanDB_Signal_1_4_LE ],
	NULL
};
const ComSignal_type * const ComIPduSignalRefs_Can_Message_3[] = {
    &ComSignal[ CanDB_Signal_1_4_LE ],
    NULL
};
const ComSignal_type * const ComIPduSignalRefs_Can_Message_4[] = {
	&ComSignal[ CanDB_Signal_1_4_LE ],
	NULL
};
/* signal group list */
const ComSignalGroup_Type  ComSignalGroup[]={
    {
      .ComHandleId=0,
      .ComUpdateBitPosition=17,
      .ComTransferProperty=TRIGGERED_ON_CHANGE,
      .ComGroupSignal=ComSignalGroup_GroupSignalsRefs1,
      .Com_Asu_ShadowBuffer=ComShadowBuffer_0
     },
     {
      .ComHandleId=1,
      .ComUpdateBitPosition=26,
      .ComTransferProperty=TRIGGERED_ON_CHANGE,
      .ComGroupSignal=ComSignalGroup_GroupSignalsRefs2,
      .Com_Asu_ShadowBuffer=ComShadowBuffer_1
     }
};


const ComTxMode_type ComTxMode[]
{
	{
		.ComTxModeMode = PERIODIC,
		.ComTxModeNumberOfRepetitions = 2,
		.ComTxModeTimePeriod=500,
		.ComTxModeRepetitionPeriod=15,
		.ComTxModeTimeOffsetFactor = 0,
		.comPeriodicTimeFired = 0
	},
	{
		.ComTxModeMode = PERIODIC,
		.ComTxModeNumberOfRepetitions = 2,
		.ComTxModeTimePeriod=500,
		.ComTxModeRepetitionPeriod=15,
		.ComTxModeTimeOffsetFactor = 0,
		.comPeriodicTimeFired = 0
	}
};

const ComTxModeTrue_type ComTxModeTrue[]
{
	{
		.ComTxMode = ComTxMode[0];
	},
	{
		.ComTxMode = ComTxMode[1];
	}
};

const ComTxModeTrue_type ComTxModeFalse[]
{
	{
		.ComTxMode = ComTxMode[1];
	},
	{
		.ComTxMode = ComTxMode[0];
	}
};

const ComTxIPdu_type ComTxIPdu[]
{
	{
		.ComMinimumDelayTime = 1,
		.ComTxIPduClearUpdateBit = CONFIRMATION,
		.ComTxIPduUnusedAreasDefault = 255,
		.ComTxModeTrue = ComTxModeTrue[0],
		.ComTxModeFalse = ComTxModeFalse[0],
		.ComNumberOfTransmissions = 0,
		.ComCurrentTransmissionSelection = 1,
		.ComIsIPduDeferred = 0,
		.ComFirstPeriodicModeEntry = 0,
		.ComFirstDirectModeEntry = 0
	},
	{
		.ComMinimumDelayTime = 1,
		.ComTxIPduClearUpdateBit = CONFIRMATION,
		.ComTxIPduUnusedAreasDefault = 255,
		.ComTxModeTrue = ComTxModeTrue[1],
		.ComTxModeFalse = ComTxModeFalse[1],
		.ComNumberOfTransmissions = 0,
		.ComCurrentTransmissionSelection = 1,
		.ComIsIPduDeferred = 0,
		.ComFirstPeriodicModeEntry = 0,
		.ComFirstDirectModeEntry = 0
	}
};

const ComIPduCounter_type ComIPduCounter[]
{
	{
		.ComIPduCounterErrorNotification = NULL,
		.ComIPduCounterSize = 8,
		.ComIPduCounterStartPosition = 16,
		.ComIPduCounterThreshold = 1,
		.ComCurrentCounterValue = 0
	},
	{
		.ComIPduCounterErrorNotification = NULL,
		.ComIPduCounterSize = 8,
		.ComIPduCounterStartPosition = 8,
		.ComIPduCounterThreshold = 1,
		.ComCurrentCounterValue = 0
	}	
};

const ComMainFunctionTx_type ComMainFunctionTx[]{
	{
		.ComMainTxTimeBase = 0,
		.ComPreparationNotification = NULL
	},
	{
		.ComMainTxTimeBase = 0,
		.ComPreparationNotification = NULL
	}	
};

const ComMainFunctionRx_type ComMainFunctionRx[]{
	{
		.ComMainRxTimeBase = 0
	},
	{
		.ComMainRxTimeBase = 0
	}		
};

/* IPdu buffers and signal */
uint8 ComIPduBuffer_1[8];

uint8 ComIPduBuffer_2[8];

uint8 ComIPduBuffer_3[8];

uint8 ComIPduBuffer_4[8];

/* Com IPdu */
const ComIPdu_type ComIPdu[] = 
{
    { // IPdu 1
        .ComIPduDirection = SEND ,
        .ComIPduHandleId = 0 ,
        .ComIPduSignalRef = ComIPduSignalRefs_Can_Message_1,
		.ComIPduDataPtr= ComIPduBuffer_1,
		.ComIPduTriggerTransmitCallout = NULL,
		.ComIPduCallout = NULL,
		.ComIPduCounter = ComIPduCounter[0],
		.ComIPduMainFunctionRef = &(ComMainFunctionTx[0]),
		.ComIPduGroupRef = &(ComIPduGroups[0]),
		.ComIPduSignalGroupRef = &(ComSignalGroup[0]),
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
        .ComTxIPdu = ComTxIPdu[0],
		.ComIPduCancellationSupport = 0
    },
	{ // IPdu 2
		.ComIPduDirection = SEND ,
		.ComIPduHandleId = 1 ,
		.ComIPduSignalRef = ComIPduSignalRefs_Can_Message_2,
		.ComIPduDataPtr= ComIPduBuffer_2,
		.ComIPduTriggerTransmitCallout = NULL,
		.ComIPduCallout = NULL,
		.ComIPduCounter = ComIPduCounter[1],
		.ComIPduMainFunctionRef = &(ComMainFunctionTx[1]),
		.ComIPduGroupRef = &(ComIPduGroups[0]),
		.ComIPduSignalGroupRef = NULL,
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
		.ComTxIPdu = ComTxIPdu[1],
		.ComIPduCancellationSupport = 0
	},
    { // IPdu 3
        .ComIPduDirection = RECEIVE ,
		.ComIPduSignalProcessing = IMMEDIATE,
        .ComIPduHandleId = 2 ,
        .ComIPduSignalRef = ComIPduSignalRefs_Can_Message_3,
		.ComIPduDataPtr=ComIPduBuffer_3,
		.ComIPduCounter = ComIPduCounter[0],
		.ComIPduMainFunctionRef = &(ComMainFunctionRx[0]),
		.ComIPduGroupRef = &(ComIPduGroups[1]),
		.ComIPduSignalGroupRef = &(ComSignalGroup[1]),
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
        .ComTxIPdu = NULL
		.ComIPduCancellationSupport = 0
    },
	{ // IPdu 4
        .ComIPduDirection = RECEIVE ,
		.ComIPduSignalProcessing = IMMEDIATE,
        .ComIPduHandleId = 3 ,
        .ComIPduSignalRef = ComIPduSignalRefs_Can_Message_4,
		.ComIPduDataPtr=ComIPduBuffer_4,
		.ComIPduCounter = ComIPduCounter[1],
        .ComIPduMainFunctionRef = &(ComMainFunctionRx[1]),
		.ComIPduGroupRef = &(ComIPduGroups[1]),
		.ComIPduSignalGroupRef = NULL,
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
		.ComTxIPdu = NULL
		.ComIPduCancellationSupport = 0
    }
    
};

const ComIPdu_type * const ComIPdusInGroup1[] = {
	&ComIPdu[0],
	&ComIPdu[1],
	NULL
};
const ComIPdu_type * const ComIPdusInGroup2[] = {
	&ComIPdu[2],
	&ComIPdu[3],
	NULL
};

const ComIPduGroup_type ComIPduGroups[] =
{
	{
		.ComIPduGroupHandleId = 0,
		.IpduGroupFlag = STOPPED,
		.IPDU = ComIPdusInGroup1,
		.numIPdus = 2
	},
	{
		.ComIPduGroupHandleId = 1,
		.IpduGroupFlag = STOPPED,
		.IPDU = ComIPdusInGroup2,
		.numIPdus = 2
	}
};


/* Com Config Container */
const ComConfig_type ComConfiguration =
{
    .ComTimeBase =
    {
         .ComRxTimeBase = 2,
         .ComTxTimeBase = 100
    },
    .ComIPdu = ComIPdu,
    .ComSignal = ComSignal,
    .ComSignalGroup=ComSignalGroup

};
