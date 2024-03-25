
#include <stdio.h>


#include "../include/Com_Cfg.h"
#include "../include/Com.h"



uint8 ComSignalBuffer_0 [1];
uint8 ComSignalBuffer_1 [1];
uint8 ComSignalBuffer_2 [4];
uint8 ComSignalBuffer_3 [2];
uint8 ComSignalBuffer_4 [1];
uint8 ComSignalBuffer_5 [4];
uint8 ComSignalBuffer_6 [1];
uint8 ComSignalBuffer_7 [4];

uint8 ComSignalFGBuffer_0 [1];
uint8 ComSignalFGBuffer_1 [1];
uint8 ComSignalFGBuffer_2 [1];
uint8 ComSignalFGBuffer_3 [1];
uint8 ComSignalFGBuffer_4 [1];
uint8 ComSignalFGBuffer_5 [1];
uint8 ComSignalFGBuffer_6 [1];
uint8 ComSignalFGBuffer_7 [1];


uint8 ComSignalBGBuffer_0 [1];
uint8 ComSignalBGBuffer_1 [1];
uint8 ComSignalBGBuffer_2 [1];
uint8 ComSignalBGBuffer_3 [1];
uint8 ComSignalBGBuffer_4 [1];
uint8 ComSignalBGBuffer_5 [1];
uint8 ComSignalBGBuffer_6 [1];
uint8 ComSignalBGBuffer_7 [1];

uint8 ComGSignalBuffer_0[1];
uint8 ComGSignalBuffer_1[1];



uint8 ComIPduBuffer_1[1];
uint8 ComIPduBuffer_2[1];
uint8 ComIPduBuffer_3[1];
uint8 ComIPduBuffer_4[1];
uint8 ComIPduBuffer_5[1];
uint8 ComIPduBuffer_6[1];


uint8 ComShadowBuffer_0[1];
uint8 ComShadowBuffer_1[1];
uint8 ComShadowBuffer_2[1];


uint8 ComFGBuffer_0[1];
uint8 ComFGBuffer_1[1];
uint8 ComFGBuffer_2[1];
uint8 ComFGBuffer_3[1];

uint8 ComBGBuffer_0[1];
uint8 ComBGBuffer_1[1];
uint8 ComBGBuffer_2[1];

uint8 ComSignalGroupBuffer_0[5];


const ComGroupSignal_type ComGSignals[2] =
{
 {    // Group signal 1
       .ComBitPosition= 32,
	   .ComBitSize= 8,
       .ComHandleId= 0 ,

	   .ComSignalEndianness = LITTLE_ENDIAN,
       .ComSignalInitValue = COM_SIGNAL_INIT_VALUE , 
	   .ComTimeoutSubstitutionValue = COM_TIMEOUT_SUBSTITUTION_VALUE, 
	   .ComSignalType = UINT16,
	 
	   .ComTransferProperty = TRIGGERED_ON_CHANGE ,
	   .ComIPduHandleId = 0,
	   .ComSignalDataPtr = ComGSignalBuffer_0,
	   .SignalGroupId = 0,
	   .IsGroupSignal = 1
   },

   //group signal 2
   {
       .ComBitPosition = 32,
	   .ComBitSize = 8,
       .ComHandleId = 1 ,
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = COM_SIGNAL_INIT_VALUE , 
	   .ComSignalType = UINT16,
	   .ComTimeoutSubstitutionValue = COM_TIMEOUT_SUBSTITUTION_VALUE, 
	   .ComTransferProperty = TRIGGERED_ON_CHANGE ,
	   .ComIPduHandleId = 0,
	   .ComSignalDataPtr = ComGSignalBuffer_1,
	   .SignalGroupId = 0,
	   .IsGroupSignal = 1
   }
   
};


const ComSignal_type ComSignal[7] = 
{

    {	//signal1
	   .ComHandleId= 0 ,
       .ComBitPosition= 0,
	   .ComBitSize= 8,

	   .ComDataInvalidAction= INVALIDATE_REPLACE,

	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, 
	   .ComSignalLength = 1,
	   .ComSignalType = BOOLEAN,
	   .ComTimeout = 0,
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_0,
	   .ComFGBuffer = ComSignalFGBuffer_0, 
	   .ComBGBuffer = ComSignalBGBuffer_0, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
    },
	{	//signal2
	   .ComHandleId= 1 ,
	   .ComBitPosition= 0,
	   .ComBitSize= 16,

	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
       
	   .ComInitialValueOnly = 0,
	   .ComInvalidNotification = NULL,
	   .ComNotification = NULL,
	 
	   .ComSignalDataInvalidValue = NULL,
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, 
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL, 
	   .ComTimeoutSubstitutionValue = NULL, 
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_1,
	   .ComFGBuffer = ComSignalFGBuffer_1, 
	   .ComBGBuffer = ComSignalBGBuffer_1, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
	},
	{	//signal3
		.ComBitPosition= 0,
	   .ComBitSize= 16,

	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
       .ComHandleId= 1,
	   .ComInitialValueOnly = 0,
	   .ComInvalidNotification = NULL,
	   .ComNotification = NULL,
	
	   .ComSignalDataInvalidValue = NULL, 
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, 
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL,
	   .ComTimeoutSubstitutionValue = NULL, 
	   .ComTransferProperty = TRIGGERED,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_2,
	   .ComFGBuffer = ComSignalFGBuffer_2, 
	   .ComBGBuffer = ComSignalBGBuffer_2, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
	},
    {	//signal4
	   .ComHandleId= 3,
       .ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
      
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, 
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL,  
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_3,
	   .ComFGBuffer = ComSignalFGBuffer_3, 
	   .ComBGBuffer = ComSignalBGBuffer_3, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
    }, 
	 {	//signal5
	   .ComHandleId= 4,
       .ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
      
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, 
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL,  
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_3,
	   .ComFGBuffer = ComSignalFGBuffer_3, 
	   .ComBGBuffer = ComSignalBGBuffer_3, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
    },

	 {	//signal6
	   .ComHandleId= 5,
       .ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
      
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, 
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL,  
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_3,
	   .ComFGBuffer = ComSignalFGBuffer_3, 
	   .ComBGBuffer = ComSignalBGBuffer_3, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
    },

	 {	//signal7
	   .ComHandleId= 6,
       .ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
      
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, 
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL,  
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_3,
	   .ComFGBuffer = ComSignalFGBuffer_3, 
	   .ComBGBuffer = ComSignalBGBuffer_3, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
    }

};


const ComFilter_type Filters[]=
{
	{
		.ComFilterAlgorithm = ALWAYS,
		.ComFilterMax= 15,
		.ComFilterMin= 5,
		.ComFilterOffset= 0,
		.ComFilterPeriod= 10,
		.ComFilterOccurrence= 0,
		.ComFilterX= 20
	}
};


const ComSignalGroup_type  ComSignalGroup[1]=
{
    {
	  
	  .ComErrorNotification = NULL,
	  .ComFirstTimeout = 0,
	  .ComHandleId=0,
	  .ComInitialValueOnly = 0,
	  .ComInvalidNotification = NULL,
	  .ComNotification = NULL,
	
	  .ComTimeout = 0,
	  .ComTimeoutNotification = NULL,
	  .signalGroupSize = 4,
	  .ComTransferProperty=TRIGGERED_ON_CHANGE,
      .ComUpdateBitPosition=17,
      .ComGroupSignal = 
	  {
		&ComGSignals[0],
		&ComGSignals[1],
		NULL

	  },
      .ComShadowBuffer = ComShadowBuffer_0, 
	  .ComFGBuffer = ComFGBuffer_0, 
	  .ComBGBuffer = ComBGBuffer_0, 
	  .ComIPduHandleId = 0,
	  .SignalGroupDataPtr = ComSignalGroupBuffer_0, 
	  .ComIsSignalGroupChanged = 0,
	  .ComSignalGroupFilterResult = 1,
	 
	}

};







const ComTxMode_type ComTxMode[3] = 
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

const ComTxModeTrue_type ComTxModeTrue[] =
{
	{
		.ComTxMode = &ComTxMode[0],
	},

	{
		.ComTxMode = &ComTxMode[1],
	}
};

const ComTxModeTrue_type ComTxModeFalse[] =
{
	{
		.ComTxMode = &ComTxMode[1],
	},
	{
		.ComTxMode = &ComTxMode[0],
	}
};

const ComTxIPdu_type ComTxIPdu[] =
{
	{
		.ComMinimumDelayTime = 1,
		.ComTxIPduClearUpdateBit = CONFIRMATION,
		.ComTxIPduUnusedAreasDefault = 255,
		.ComTxModeTrue = &ComTxModeTrue[0],
		.ComTxModeFalse = &ComTxModeFalse[0],
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
		.ComTxModeTrue = &ComTxModeTrue[1],
		.ComTxModeFalse = &ComTxModeFalse[1],
		.ComNumberOfTransmissions = 0,
		.ComCurrentTransmissionSelection = 1,
		.ComIsIPduDeferred = 0,
		.ComFirstPeriodicModeEntry = 0,
		.ComFirstDirectModeEntry = 0
	}
};

const ComIPduCounter_type ComIPduCounter[2] =
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

const ComMainFunctionTx_type ComMainFunctionTx[]=
{
	{
		.ComMainTxTimeBase = 0,
		.ComPreparationNotification = NULL
	},
	{
		.ComMainTxTimeBase = 0,
		.ComPreparationNotification = NULL
	}	
};

const ComMainFunctionRx_type ComMainFunctionRx[]=
{
	{
		.ComMainRxTimeBase = 0
	},
	{
		.ComMainRxTimeBase = 0
	}		
};


const ComIPdu_type ComIPdu[6] = 
{
    {   // IPdu 1
	    .ComIPduHandleId = 0 ,
        .ComIPduDirection = SEND ,
		.ComIPduType = NORMAL,

        .ComIPduSignalRef =
		 {
            &ComSignal[0],
			&ComSignal[1],
		 },

		.ComIPduDataPtr = ComIPduBuffer_1,
		.ComIPduTriggerTransmitCallout = NULL,
		.ComIPduCallout = NULL,
		.ComIPduCounter = &ComIPduCounter[0],
		.ComIPduMainFunctionRef = &(ComMainFunctionTx[0]),
		.ComIPduGroupRef = &(ComIPduGroups[0]),
		.ComIPduSignalGroupRef = &(ComSignalGroup[0]),
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
        .ComTxIPdu = &ComTxIPdu[0],
		.ComIPduCancellationSupport = 0
    },

	{ // IPdu 2
		.ComIPduDirection = SEND ,
		.ComIPduHandleId = 1 ,
		.ComIPduSignalRef = 
		{
			&ComSignal[2],
			&ComSignal[3],
		},
		.ComIPduDataPtr = ComIPduBuffer_2,
		.ComIPduTriggerTransmitCallout = NULL,
		.ComIPduCallout = NULL,
		.ComIPduCounter = &ComIPduCounter[1],
		.ComIPduMainFunctionRef = &(ComMainFunctionTx[1]),
		.ComIPduGroupRef = &(ComIPduGroups[0]),
		.ComIPduSignalGroupRef = NULL,
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
		.ComTxIPdu = &ComTxIPdu[1],
		.ComIPduCancellationSupport = 0
	},
    { // IPdu 3
        .ComIPduDirection = SEND ,
		.ComIPduSignalProcessing = IMMEDIATE,
        .ComIPduHandleId = 2 ,
        .ComIPduSignalRef = ComIPduBuffer_3,
		.ComIPduDataPtr=ComIPduBuffer_3,
		.ComIPduCounter = &ComIPduCounter[0],

		.ComIPduMainFunctionRef = &(ComMainFunctionRx[0]),
		.ComIPduGroupRef = NULL,
		.ComIPduSignalGroupRef = &(ComSignalGroup[1]),
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
        .ComTxIPdu = NULL,
		.ComIPduCancellationSupport = 0
    },
	{ // IPdu 4

        .ComIPduDirection = RECEIVE ,
		.ComIPduSignalProcessing = IMMEDIATE,
        .ComIPduHandleId = 3 ,
        .ComIPduSignalRef = ComIPduBuffer_4,
		.ComIPduDataPtr=ComIPduBuffer_4,
		.ComIPduCounter = &ComIPduCounter[1],
        .ComIPduMainFunctionRef = &(ComMainFunctionRx[1]),
		.ComIPduGroupRef = NULL,
		.ComIPduSignalGroupRef = NULL,
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
		.ComTxIPdu = NULL,
		.ComIPduCancellationSupport = 0
    }

	



    
};




const ComIPduGroup_type ComIPduGroups[1] =
{
	{
		.ComIPduGroupHandleId = 0,
		.IpduGroupFlag = STOPPED,
	}
	
};



const Com_ConfigType ComConfig =
{

    .ComIPdu = ComIPdu,
    .ComSignal = ComSignal,
    .ComSignalGroup = ComSignalGroup

};
