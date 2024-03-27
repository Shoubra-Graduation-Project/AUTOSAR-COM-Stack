
#include <stdio.h>
#include "../include/Com_Cfg.h"

#include "../include/Com_types.h"
#include "../include/ComStack_Types.h"



uint8 ComSignalBuffer_0 [1];
uint8 ComSignalBuffer_1 [1];
uint8 ComSignalBuffer_2 [1];
uint8 ComSignalBuffer_3 [1];
uint8 ComSignalBuffer_4 [1];
uint8 ComSignalBuffer_5 [1];
uint8 ComSignalBuffer_6 [1];
uint8 ComSignalBuffer_7 [1];

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

uint8 ComGSignalFGBuffer_0[1];
uint8 ComGSignalFGBuffer_1[1];


uint8 ComIPduBuffer_1[8];
uint8 ComIPduBuffer_2[8];
uint8 ComIPduBuffer_3[8];
uint8 ComIPduBuffer_4[8];
uint8 ComIPduBuffer_5[8];
uint8 ComIPduBuffer_6[8];


uint8 ComShadowBuffer_0[4];
uint8 ComShadowBuffer_1[4];
uint8 ComShadowBuffer_2[4];


uint8 ComFGBuffer_0[4];
uint8 ComFGBuffer_1[4];
uint8 ComFGBuffer_2[4];
uint8 ComFGBuffer_3[4];

uint8 ComBGBuffer_0[4];
uint8 ComBGBuffer_1[4];
uint8 ComBGBuffer_2[4];

uint8 ComSignalGroupBuffer_0[5];

PduIdType PduIDs[4];

ComFilter_type Filters[1]=
{
	{
<<<<<<< HEAD
		.ComFilterAlgorithm = ALWAYS,
		.ComFilterMax= 15,
		.ComFilterMin= 5,
		.ComFilterOffset= 0,
		.ComFilterPeriod= 10,
		.ComFilterOccurrence= 0,
		.ComFilterX= 20
	},
=======
		ALWAYS,
		15,
		5,
		0,
		10,
		0,
		20
	}
>>>>>>> cb594e3a889349781e1011a25b599edd89b9f562
};

const ComGroupSignal_type ComGSignals[2] =
{
		
   {    
<<<<<<< HEAD
       .ComBitPosition = 0,
	   .ComBitSize= 8,
       .ComHandleId= 0 ,
		.ComGlobalGroupSignalID = 0,
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalType = UINT16,
	   .ComTransferProperty = TRIGGERED_ON_CHANGE ,
	   .ComIPduHandleId = 0,
	   .ComSignalDataPtr = ComGSignalBuffer_0,
	   .SignalGroupId = 0,
		.ComFGBuffer = ComGSignalFGBuffer_0
=======
       0, 16, 0, NULL, LITTLE_ENDIAN, NULL, 1, UINT16, NULL, TRIGGERED, 0, ComGSignalBuffer_0, 0, 0, ComGSignalFGBuffer_0
>>>>>>> cb594e3a889349781e1011a25b599edd89b9f562
   },
		
   {
       32, 8, 0, NULL, LITTLE_ENDIAN, NULL, 1, UINT8, NULL, TRIGGERED, 2, ComGSignalBuffer_1, 1, 1, ComGSignalFGBuffer_1
   }
	 
   
};


const ComSignal_type ComSignal[8] = 
{
    {	//signal1
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 0, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
			  NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 8, ComSignalBuffer_0, ComSignalFGBuffer_0, ComSignalBGBuffer_0, 0, 0, 0, 1, 0, &Filters[0]
    },
		{	//signal2
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 1, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED_ON_CHANGE, 40, ComSignalBuffer_1, ComSignalFGBuffer_1, ComSignalBGBuffer_1, 0, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal3
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 2, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 8, ComSignalBuffer_2, ComSignalFGBuffer_2, ComSignalBGBuffer_2, 1, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal4
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 3, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, PENDING, 40, ComSignalBuffer_3, ComSignalFGBuffer_3, ComSignalBGBuffer_3, 1, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal5
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 4, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 8, ComSignalBuffer_4, ComSignalFGBuffer_4, ComSignalBGBuffer_4, 2, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal6
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 5, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 40, ComSignalBuffer_5, ComSignalFGBuffer_5, ComSignalBGBuffer_5, 2, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal7
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 6, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED_ON_CHANGE, 8, ComSignalBuffer_6, ComSignalFGBuffer_6, ComSignalBGBuffer_6, 3, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal8
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 7, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 40, ComSignalBuffer_7, ComSignalFGBuffer_7, ComSignalBGBuffer_7, 3, 0, 0, 1, 0, &Filters[0]
		}

};

const ComGroupSignal_type* GroupSignalListSignalGroup_0[] =
{
	&ComGSignals[0],
		NULL
};

const ComGroupSignal_type* GroupSignalListSignalGroup_1[] =
{
	&ComGSignals[1],
		NULL
};

const ComSignalGroup_type  ComSignalGroup[2]=
{
  {
			INVALIDATE_REPLACE, NULL, 0, 0, 0, NULL, NULL, TIMEOUT_REPLACE, 0, NULL, 1, TRIGGERED, 24,
			GroupSignalListSignalGroup_0, ComShadowBuffer_0, ComFGBuffer_0, ComBGBuffer_0, 0, &ComSignalGroupBuffer_0[0], 0, 1, 0, &Filters[0]
	},
	{
			INVALIDATE_REPLACE, NULL, 0, 0, 0, NULL, NULL, TIMEOUT_REPLACE, 0, NULL, 1, TRIGGERED, 24,
			GroupSignalListSignalGroup_1, ComShadowBuffer_1, ComFGBuffer_1, ComBGBuffer_1, 0, &ComSignalGroupBuffer_0[1], 0, 1, 0, &Filters[0]
	}

};



ComTxMode_type ComTxMode[2] = 
{
	{
		PERIODIC, 2, 15, 0, 500, 0
	},
	{
		DIRECT, 2, 15, 0, 500, 0
	},

};

ComTxModeTrue_type ComTxModeTrue[2] =
{
	{
		&ComTxMode[0],
	},

	{
		&ComTxMode[1],
	}
};

ComTxModeFalse_type ComTxModeFalse[2] =
{
	{
		&ComTxMode[1],
	},
	{
		&ComTxMode[0],
	}
};

ComTxIPdu_type ComTxIPdu[] =
{
	{
		1, CONFIRMATION, 255, &ComTxModeTrue[0], &ComTxModeFalse[0], 0, 1, 0, 0, 0
	},
	{
		1, CONFIRMATION, 255, &ComTxModeTrue[1], &ComTxModeFalse[1], 0, 1, 0, 0, 0
	}
};

ComIPduCounter_type ComIPduCounter[2] =
{
	{
		NULL, 8, 10, 1, 0
	},
	{
		NULL, 4, 10, 1, 0
	}	
};

const ComMainFunctionTx_type ComMainFunctionTx[]=
{
	{
		0, NULL
	}
};

const ComMainFunctionRx_type ComMainFunctionRx[]=
{
	{
		0
	}	
};

ComIPduGroup_type ComIPduGroups[1] =
{
	{
		0, STOPPED, 0, 2
	}
	
};

const ComSignal_type* SignalListIPdu_0[] =
{
	&ComSignal[0],
	&ComSignal[1],
		NULL
};

const ComSignal_type* SignalListIPdu_1[] =
{
	&ComSignal[2],
	&ComSignal[3],
		NULL
};

const ComSignal_type* SignalListIPdu_2[] =
{
	&ComSignal[4],
	&ComSignal[5],
		NULL
};

const ComSignal_type* SignalListIPdu_3[] =
{
	&ComSignal[6],
	&ComSignal[7],
		NULL
};

const ComSignalGroup_type* SignalGroupListIPdu_0[] =
{
	&ComSignalGroup[0],
		NULL
};

const ComSignalGroup_type* SignalGroupListIPdu_2[] =
{
	&ComSignalGroup[1],
		NULL
};

const ComIPdu_type ComIPdu[6] = 
{
<<<<<<< HEAD
  {   // IPdu 1
	    .ComIPduHandleId = 0 ,
        .ComIPduDirection = SEND ,
		.ComIPduType = NORMAL,
        .ComIPduSignalRef = SignalListIPdu_0,
		.ComIPduDataPtr = ComIPduBuffer_1,
		.ComIPduTriggerTransmitCallout = NULL,
		.ComIPduCallout = NULL,
		.ComIPduCounter = &ComIPduCounter[0],
		.ComIPduGroupRef = &(ComIPduGroups[0]),
		.ComIPduSignalGroupRef = SignalGroupListIPdu_0,
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
    .ComTxIPdu = &ComTxIPdu[0],
		.ComIPduCancellationSupport = 0
    },

=======
  { // IPdu 1
	  0, &ComIPduCounter[0], SEND, 0, DEFERRED, NULL, NORMAL, NULL, NULL, &ComIPduGroups[0], SignalGroupListIPdu_0, 
		SignalListIPdu_0, &PduIDs[0], &ComTxIPdu[0], ComIPduBuffer_1, 64, 0
  },
>>>>>>> cb594e3a889349781e1011a25b599edd89b9f562
	{ // IPdu 2
		0, &ComIPduCounter[1], SEND, 1, DEFERRED, NULL, NORMAL, NULL, NULL, &ComIPduGroups[0], NULL, SignalListIPdu_1, 
		&PduIDs[1], &ComTxIPdu[0], ComIPduBuffer_2, 64, 0
	},
  { // IPdu 3
    0, &ComIPduCounter[0], RECEIVE, 2, DEFERRED, NULL, NORMAL, NULL, NULL, NULL, SignalGroupListIPdu_2, SignalListIPdu_2, 
		&PduIDs[2], &ComTxIPdu[1], ComIPduBuffer_3, 64, 0
  },
	{ // IPdu 4
<<<<<<< HEAD

    .ComIPduDirection = RECEIVE ,
	.ComIPduSignalProcessing = IMMEDIATE,
    .ComIPduHandleId = 3 ,
    .ComIPduSignalRef = SignalListIPdu_3,
		.ComIPduDataPtr=ComIPduBuffer_4,
		.ComIPduCounter = &ComIPduCounter[1],
		.ComIPduGroupRef = NULL,
		.ComIPduSignalGroupRef = NULL,
		.ComIPduLength = 8,
		.ReceptionDMEnabled = 0,
		.ComTxIPdu = NULL,
		.ComIPduCancellationSupport = 0
    }
=======
		0, &ComIPduCounter[1], RECEIVE, 3, DEFERRED, NULL, NORMAL, NULL, NULL, NULL, NULL, SignalListIPdu_3, &PduIDs[3], 
		&ComTxIPdu[1], ComIPduBuffer_4, 64, 0
  }
>>>>>>> cb594e3a889349781e1011a25b599edd89b9f562

};

const ComIPdu_type* IPduList[] =
{
	&ComIPdu[0],
	&ComIPdu[1],
	&ComIPdu[2],
	&ComIPdu[3],
	NULL
};

const ComIPduGroup_type* IPduGroupList[] =
{
	&ComIPduGroups[0],
	NULL
};

const ComSignal_type* SignalList[] =
{
	&ComSignal[0],
	&ComSignal[1],
	&ComSignal[2],
	&ComSignal[3],
	&ComSignal[4],
	&ComSignal[5],
	&ComSignal[6],
	&ComSignal[7],
	NULL
};

const ComSignalGroup_type* SignalGroupList[] =
{
	&ComSignalGroup[0],
	&ComSignalGroup[1],
	NULL
};

const ComGroupSignal_type* GroupSignalList[] =
{
	&ComGSignals[0],
	&ComGSignals[1],
	NULL
};

const Com_ConfigType ComConfig =
{

<<<<<<< HEAD
    .ComIPdu = IPduList,
    .ComSignal = SignalList,
    .ComSignalGroup = SignalGroupList,
	.ComGroupSignal = GroupSignalList
=======
    50000, COM_NUM_OF_IPDU, IPduList, IPduGroupList, SignalList, SignalGroupList, GroupSignalList
>>>>>>> cb594e3a889349781e1011a25b599edd89b9f562

};


