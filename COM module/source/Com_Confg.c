
#include <stdio.h>
#include "../include/Com_Cfg.h"
#include "../include/Com.h"
#include "../include/Com_types.h"
#include "../../Common/ComStack_Types.h"
#include "../../Timer Driver/include/Timer.h"


uint8 ComSignalBuffer_0 [1] = {0};
uint8 ComSignalBuffer_1 [1] = {0};
uint8 ComSignalBuffer_2 [1] = {0};
uint8 ComSignalBuffer_3 [1] = {0};
uint8 ComSignalBuffer_4 [1] = {0};
uint8 ComSignalBuffer_5 [1] = {0};
uint8 ComSignalBuffer_6 [1] = {0};
uint8 ComSignalBuffer_7 [1] = {0};

uint8 ComSignalFGBuffer_0 [1] = {0};
uint8 ComSignalFGBuffer_1 [1] = {0};
uint8 ComSignalFGBuffer_2 [1] = {0};
uint8 ComSignalFGBuffer_3 [1] = {0};
uint8 ComSignalFGBuffer_4 [1] = {0};
uint8 ComSignalFGBuffer_5 [1] = {0};
uint8 ComSignalFGBuffer_6 [1] = {0};
uint8 ComSignalFGBuffer_7 [1] = {0};


uint8 ComSignalBGBuffer_0 [1] = {0};
uint8 ComSignalBGBuffer_1 [1] = {0};
uint8 ComSignalBGBuffer_2 [1] = {0};
uint8 ComSignalBGBuffer_3 [1] = {0};
uint8 ComSignalBGBuffer_4 [1] = {0};
uint8 ComSignalBGBuffer_5 [1] = {0};
uint8 ComSignalBGBuffer_6 [1] = {0};
uint8 ComSignalBGBuffer_7 [1] = {0};

uint8 ComGSignalBuffer_0[1] = {0};
uint8 ComGSignalBuffer_1[1] = {0};

uint8 ComGSignalFGBuffer_0[1] = {0};
uint8 ComGSignalFGBuffer_1[1] = {0};


uint8 ComIPduBuffer_1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8 ComIPduBuffer_2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8 ComIPduBuffer_3[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8 ComIPduBuffer_4[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8 ComIPduBuffer_5[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8 ComIPduBuffer_6[8] = {0, 0, 0, 0, 0, 0, 0, 0};


uint8 ComShadowBuffer_0[4] = {0, 0, 0, 0};
uint8 ComShadowBuffer_1[4] = {0, 0, 0, 0};
uint8 ComShadowBuffer_2[4] = {0, 0, 0, 0};


uint8 ComFGBuffer_0[4] = {0, 0, 0, 0};
uint8 ComFGBuffer_1[4] = {0, 0, 0, 0};
uint8 ComFGBuffer_2[4] = {0, 0, 0, 0};
uint8 ComFGBuffer_3[4] = {0, 0, 0, 0};

uint8 ComBGBuffer_0[4] = {0, 0, 0, 0};
uint8 ComBGBuffer_1[4] = {0, 0, 0, 0};
uint8 ComBGBuffer_2[4] = {0, 0, 0, 0};

uint8 ComSignalGroupBuffer_0[5] = {0, 0, 0, 0, 0};

PduIdType PduIDs[4];

ComFilter_type Filters[1]=
{
	{
		ALWAYS,
		15,
		5,
		0,
		10,
		0,
		20
	}
};

ComGroupSignal_type ComGSignals[4] =
{
		
   {    
       10, 8, 0, NULL, LITTLE_ENDIAN, NULL, 1, UINT8, NULL, TRIGGERED, 0, ComGSignalBuffer_0, 0, 0, ComGSignalFGBuffer_0,TIMEOUT_REPLACE, 0
   },
		
   {
       10, 8, 0, NULL, LITTLE_ENDIAN, NULL, 1, UINT8, NULL, TRIGGERED, 2, ComGSignalBuffer_1, 1, 1, ComGSignalFGBuffer_1,TIMEOUT_REPLACE, 0
   },
		
   {
       10, 8, 0, NULL, LITTLE_ENDIAN, NULL, 1, UINT8, NULL, TRIGGERED, 2, ComGSignalBuffer_1, 1, 2, ComGSignalFGBuffer_1,TIMEOUT_REPLACE, 0
   },
		
   {
       10, 8, 0, NULL, LITTLE_ENDIAN, NULL, 1, UINT8, NULL, TRIGGERED, 2, ComGSignalBuffer_1, 1, 3, ComGSignalFGBuffer_1,TIMEOUT_REPLACE, 0
   }
	 
   
};


ComSignal_type ComSignal[8] = 
{
    {	//signal1
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 32768, 0, NULL, NULL, TIMEOUT_REPLACE, "A", LITTLE_ENDIAN, 
			  NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 8, ComSignalBuffer_0, ComSignalFGBuffer_0, ComSignalBGBuffer_0, 0, 0, 0, 1, 0, &Filters[0]
    },
		{	//signal2
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 32769, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED_ON_CHANGE, 40, ComSignalBuffer_1, ComSignalFGBuffer_1, ComSignalBGBuffer_1, 0, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal3
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 32770, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 8, ComSignalBuffer_2, ComSignalFGBuffer_2, ComSignalBGBuffer_2, 1, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal4
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 32771, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, PENDING, 40, ComSignalBuffer_3, ComSignalFGBuffer_3, ComSignalBGBuffer_3, 1, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal5
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 32772, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 8, ComSignalBuffer_4, ComSignalFGBuffer_4, ComSignalBGBuffer_4, 2, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal6
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 32773, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 40, ComSignalBuffer_5, ComSignalFGBuffer_5, ComSignalBGBuffer_5, 2, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal7
			 0, 8, INVALIDATE_REPLACE, NULL, 0, 32774, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED_ON_CHANGE, 8, ComSignalBuffer_6, ComSignalFGBuffer_6, ComSignalBGBuffer_6, 3, 0, 0, 1, 0, &Filters[0]
		},
		{	//signal8
			 32, 8, INVALIDATE_REPLACE, NULL, 0, 32775, 0, NULL, NULL, TIMEOUT_REPLACE, NULL, LITTLE_ENDIAN, 
				NULL, 1, UINT8, 0, NULL, NULL, TRIGGERED, 40, ComSignalBuffer_7, ComSignalFGBuffer_7, ComSignalBGBuffer_7, 3, 0, 0, 1, 0, &Filters[0]
		}

};

ComGroupSignal_type* GroupSignalListSignalGroup_0[] =
{
	&ComGSignals[0],
		NULL
};

ComGroupSignal_type* GroupSignalListSignalGroup_1[] =
{
	&ComGSignals[1],
		NULL
};

ComSignalGroup_type  ComSignalGroup[2]=
{
  {
			INVALIDATE_REPLACE, NULL, 0, 0, 0, NULL, NULL, TIMEOUT_REPLACE, 0, NULL, 2, TRIGGERED, 24,
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
		1, CONFIRMATION, 0, &ComTxModeTrue[0], &ComTxModeFalse[0], 0, 1, 0, 0, TIMERS_TIMER0A, 'A', 0
	},
	{
		1, CONFIRMATION, 0, &ComTxModeTrue[1], &ComTxModeFalse[1], 0, 1, 0, 0, TIMERS_TIMER0A, 'A', 0
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

ComMainFunctionTx_type ComMainFunctionTx[]=
{
	{
		0, NULL
	}
};

ComMainFunctionRx_type ComMainFunctionRx[]=
{
	{
		0
	}	
};

ComIPduGroup_type ComIPduGroups[1] =
{
	{
		0, STARTED, 0, 2
	}
	
};

ComSignal_type* SignalListIPdu_0[] =
{
	&ComSignal[0],
	&ComSignal[1],
		NULL
};

ComSignal_type* SignalListIPdu_1[] =
{
	&ComSignal[2],
	&ComSignal[3],
		NULL
};

ComSignal_type* SignalListIPdu_2[] =
{
	&ComSignal[4],
	&ComSignal[5],
		NULL
};

ComSignal_type* SignalListIPdu_3[] =
{
	&ComSignal[6],
	&ComSignal[7],
		NULL
};

ComSignalGroup_type* SignalGroupListIPdu_0[] =
{
	&ComSignalGroup[0],
		NULL
};

ComSignalGroup_type* SignalGroupListIPdu_1[] =
{
	&ComSignalGroup[1],
		NULL
};

ComSignalGroup_type* SignalGroupListIPdu_2[] =
{
	&ComSignalGroup[2],
		NULL
};

ComSignalGroup_type* SignalGroupListIPdu_3[] =
{
	&ComSignalGroup[3],
		NULL
};

PduInfoType PduInfo[4] =
{
	{
		68, (void *)ComIPduBuffer_1
	},
	{
		68, (void *)ComIPduBuffer_2
	},
	{
		68, (void *)ComIPduBuffer_3
	},
	{
		68, (void *)ComIPduBuffer_4
	}
};

ComIPdu_type ComIPdu[6] = 
{
  { // IPdu 1
	  0, &ComIPduCounter[0], COM_SEND, 0, DEFERRED, NULL, NORMAL, NULL, NULL, &ComIPduGroups[0], SignalGroupListIPdu_0, 
		SignalListIPdu_0, &PduIDs[0], &ComTxIPdu[0], ComIPduBuffer_1, 64, 0, &PduInfo[0]
  },
	{ // IPdu 2
		0, &ComIPduCounter[1], COM_SEND, 1, DEFERRED, NULL, NORMAL, NULL, NULL, &ComIPduGroups[0], SignalGroupListIPdu_1, SignalListIPdu_1, 
		&PduIDs[1], &ComTxIPdu[0], ComIPduBuffer_2, 64, 0, &PduInfo[1]
	},
  { // IPdu 3
    0, &ComIPduCounter[0], COM_RECEIVE, 2, DEFERRED, NULL, NORMAL, NULL, NULL, NULL, SignalGroupListIPdu_2, SignalListIPdu_2, 
		&PduIDs[2], &ComTxIPdu[1], ComIPduBuffer_3, 64, 0, &PduInfo[2]
  },
	{ // IPdu 4
		0, &ComIPduCounter[1], COM_RECEIVE, 3, DEFERRED, NULL, NORMAL, NULL, NULL, NULL, SignalGroupListIPdu_3, SignalListIPdu_3, &PduIDs[3], 
		&ComTxIPdu[1], ComIPduBuffer_4, 64, 0, &PduInfo[3]
  }

};



ComIPdu_type* IPduList[] =
{
	&ComIPdu[0],
	&ComIPdu[1],
	&ComIPdu[2],
	&ComIPdu[3],
	NULL
};

ComIPduGroup_type* IPduGroupList[] =
{
	&ComIPduGroups[0],
	NULL
};

ComSignal_type* SignalList[] =
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

ComSignalGroup_type* SignalGroupList[] =
{
	&ComSignalGroup[0],
	&ComSignalGroup[1],
	NULL
};

ComGroupSignal_type* GroupSignalList[] =
{
	&ComGSignals[0],
	&ComGSignals[1],
	NULL
};

 Com_ConfigType ComConfig =
{

    50000, COM_NUM_OF_IPDU, IPduList, IPduGroupList, SignalList, SignalGroupList, GroupSignalList

};