
/*
 * Copyright (c) 2018-2019 Faculty of Engineering Ain Shams University Dr.Sherif Hammad Email: Sherif.hammad@eng.asu.edu.eg Eng.Mohamed abdelhay Email: mhmd.a.elhay@gmail.com.
 *
 * This file is part of ASUSAR
 * (see https://github.com/asusar/communication-stack).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#include <stdio.h>
#include "../libraries/Std_Types.h"
#include "../include/Com_Types.h"
#include "../include/Com_Cfg.h"

/* signals Buffer */
uint8 ComSignalBuffer_0 [2];
uint8 ComSignalBuffer_1 [2];
uint8 ComSignalBuffer_2 [2];
uint8 ComSignalBuffer_3 [2];
/* signal Groups Buffer */
uint8 ComSignalGroupBuffer_0 [4];
uint8 ComSignalGroupBuffer_1 [4];

/* signals' FG Buffer */
uint8 ComSignalFGBuffer_0 [2];
uint8 ComSignalFGBuffer_1 [2];
uint8 ComSignalFGBuffer_2 [2];
uint8 ComSignalFGBuffer_3 [2];
/* signals' BG Buffer */
uint8 ComSignalBGBuffer_0 [2];
uint8 ComSignalBGBuffer_1 [2];
uint8 ComSignalBGBuffer_2 [2];
uint8 ComSignalBGBuffer_3 [2];
/* Com Group Signal Buffer  */
uint8 ComGSignalBuffer_0 [1];
uint8 ComGSignalBuffer_1 [2];
/* shadow buffers */
uint8 ComShadowBuffer_0 [4];
uint8 ComShadowBuffer_1 [4];
/* signal Groups' FG Buffer */
uint8 ComFGBuffer_0 [4];
uint8 ComFGBuffer_1 [4];
/* signal Groups' BG Buffer */
uint8 ComBGBuffer_0 [4];
uint8 ComBGBuffer_1 [4];
/* IPdu buffers and signal */
uint8 ComIPduBuffer_1[8];
uint8 ComIPduBuffer_2[8];
uint8 ComIPduBuffer_3[8];
uint8 ComIPduBuffer_4[8];



const ComGroupSignal_type ComGSignals[]=
{
 {    // Group signal 1
       .ComBitPosition= 32,
	   .ComBitSize= 8,
       .ComHandleId= 0 ,
	   .ComSignalDataInvalidValue = , //-------------------------------->??
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = , //-------------------------------->??
	   .ComSignalType = UINT16,
	   .ComTimeoutSubstitutionValue = , //-------------------------------->??
	   .ComTransferProperty = TRIGGERED_ON_CHANGE ,
	   .ComIPduHandleId = 0,
	   .ComSignalDataPtr = ComGSignalBuffer_0,
	   .SignalGroupId = 0,
	   .IsGroupSignal = 1
   },
   //group signal 2
   {
    .ComBitPosition= 32,
	   .ComBitSize= 8,
       .ComHandleId= 0 ,
	   .ComSignalDataInvalidValue = , //-------------------------------->??
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = , //-------------------------------->??
	   .ComSignalType = UINT16,
	   .ComTimeoutSubstitutionValue = , //-------------------------------->??
	   .ComTransferProperty = TRIGGERED_ON_CHANGE ,
	   .ComIPduHandleId = 0,
	   .ComSignalDataPtr = ComGSignalBuffer_1,
	   .SignalGroupId = 0,
	   .IsGroupSignal = 1
   }
   
};


const ComSignal_type ComSignal[] = 
{

    {	//signal1
       .ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComDataInvalidAction= REPLACE,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
       .ComHandleId= 0 ,
	   .ComInitialValueOnly = 0,
	   .ComInvalidNotification = NULL,
	   .ComNotification = NULL,
	   .ComRxDataTimeoutAction = REPLACE,
	   .ComSignalDataInvalidValue = NULL, //-------------------------------->??
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, //-------------------------------->??
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL, 
	   .ComTimeoutSubstitutionValue = NULL, //-------------------------------->??
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_0,
	   .ComFGBuffer = ComSignalFGBuffer_0, 
	   .ComBGBuffer = ComSignalBGBuffer_0, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .containingIPDU = &ComIPdu[0],
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
    },
	{	//signal2
		.ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComDataInvalidAction= REPLACE,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
       .ComHandleId= 1 ,
	   .ComInitialValueOnly = 0,
	   .ComInvalidNotification = NULL,
	   .ComNotification = NULL,
	   .ComRxDataTimeoutAction = REPLACE,
	   .ComSignalDataInvalidValue = NULL, //-------------------------------->??
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, //-------------------------------->??
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL, 
	   .ComTimeoutSubstitutionValue = NULL, //-------------------------------->??
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_1,
	   .ComFGBuffer = ComSignalFGBuffer_1, 
	   .ComBGBuffer = ComSignalBGBuffer_1, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .containingIPDU = &ComIPdu[1],
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
	},
	{	//signal3
		.ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComDataInvalidAction= REPLACE,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
       .ComHandleId= 1,
	   .ComInitialValueOnly = 0,
	   .ComInvalidNotification = NULL,
	   .ComNotification = NULL,
	   .ComRxDataTimeoutAction = REPLACE,
	   .ComSignalDataInvalidValue = NULL, //-------------------------------->??
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, //-------------------------------->??
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL, 
	   .ComTimeoutSubstitutionValue = NULL, //-------------------------------->??
	   .ComTransferProperty = TRIGGERED,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_2,
	   .ComFGBuffer = ComSignalFGBuffer_2, 
	   .ComBGBuffer = ComSignalBGBuffer_2, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .containingIPDU = &ComIPdu[2],
	   .ComSignalUpdated = 0,
	   .ComFilter = &Filters[0]
	},
    {	//signal4
        .ComBitPosition= 0,
	   .ComBitSize= 16,
	   .ComDataInvalidAction= REPLACE,
	   .ComErrorNotification = NULL,
	   .ComFirstTimeout = 0,
       .ComHandleId= 3,
	   .ComInitialValueOnly = 0,
	   .ComInvalidNotification = NULL,
	   .ComNotification = NULL,
	   .ComRxDataTimeoutAction = REPLACE,
	   .ComSignalDataInvalidValue = NULL, //-------------------------------->??
	   .ComSignalEndianness = LITTLE_ENDIAN,
	   .ComSignalInitValue = NULL, //-------------------------------->??
	   .ComSignalLength = 1,
	   .ComSignalType = UINT16,
	   .ComTimeout = 0,
	   .ComTimeoutNotification = NULL, 
	   .ComTimeoutSubstitutionValue = NULL, //-------------------------------->??
	   .ComTransferProperty = TRIGGERED ,
	   .ComUpdateBitPosition = 16,
       .ComSignalDataPtr= ComSignalBuffer_3,
	   .ComFGBuffer = ComSignalFGBuffer_3, 
	   .ComBGBuffer = ComSignalBGBuffer_3, 
	   .ComIPduHandleId = 0,
	   .IsGroupSignal = 0,
	   .ComIsSignalChanged = 0,
	   .ComSignalFilterResult = 1,
	   .containingIPDU = &ComIPdu[3],
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


/* signal group list */
const ComSignalGroup_type  ComSignalGroup[]=
{
    {
	  .ComDataInvalidAction = REPLACE,
	  .ComErrorNotification = NULL,
	  .ComFirstTimeout = 0,
	  .ComHandleId=0,
	  .ComInitialValueOnly = 0,
	  .ComInvalidNotification = NULL,
	  .ComNotification = NULL,
	  .ComRxDataTimeoutAction = REPLACE,
	  .ComTimeout = 0,
	  .ComTimeoutNotification = NULL,
	  .signalGroupSize = 4,
	  .ComTransferProperty=TRIGGERED_ON_CHANGE,
      .ComUpdateBitPosition=17,
      .ComGroupSignal = ComSignalGroup_1,
      .ComShadowBuffer = ComShadowBuffer_0, 
	  .ComFGBuffer = ComFGBuffer_0, 
	  .ComBGBuffer = ComBGBuffer_0, 
	  .ComIPduHandleId = 0,
	  .SignalGroupDataPtr = ComSignalGroupBuffer_0, 
	  .ComIsSignalGroupChanged = 0,
	  .ComSignalGroupFilterResult = 1,
	  .ComFilter = &Filters[0]
     },
     {
      .ComDataInvalidAction = REPLACE,
	  .ComErrorNotification = NULL,
	  .ComFirstTimeout = 0,
	  .ComHandleId=1,
	  .ComInitialValueOnly = 0,
	  .ComInvalidNotification = NULL,
	  .ComNotification = NULL,
	  .ComRxDataTimeoutAction = REPLACE,
	  .ComTimeout = 0,
	  .ComTimeoutNotification = NULL,
	  .signalGroupSize = 4,
	  .ComTransferProperty=TRIGGERED_ON_CHANGE,
      .ComUpdateBitPosition=17,
      .ComGroupSignal = ComSignalGroup_2,
      .ComShadowBuffer = ComShadowBuffer_1, 
	  .ComFGBuffer = ComFGBuffer_1, 
	  .ComBGBuffer = ComBGBuffer_1, 
	  .ComIPduHandleId = 2,
	  .SignalGroupDataPtr = ComSignalGroupBuffer_0, 
	  .ComIsSignalGroupChanged = 0,
	  .ComSignalGroupFilterResult = 1,
	  .ComFilter = &Filters[0]
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
const ComGroupSignal_type * const ComSignalGroup_1[]={
     &ComGSignals[ 0 ],
     NULL
};

const ComGroupSignal_type * const ComSignalGroup_2[]={
     &ComGSignals[ 1 ],
     NULL
};

/* IPdu signal lists. */
const ComSignal_type * const ComIPduSignalRefs_Can_Message_1[] = {
    &ComSignal[ 0 ],
	NULL
};

const ComSignal_type * const ComIPduSignalRefs_Can_Message_2[] = {
    &ComSignal[ 1 ],
	NULL
};
const ComSignal_type * const ComIPduSignalRefs_Can_Message_3[] = {
    &ComSignal[ 2 ],
    NULL
};
const ComSignal_type * const ComIPduSignalRefs_Can_Message_4[] = {
	&ComSignal[ 3 ],
	NULL
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

const ComMainFunctionRx_type ComMainFunctionRx[]
{
	{
		.ComMainRxTimeBase = 0
	},
	{
		.ComMainRxTimeBase = 0
	}		
};

/* Com IPdu */
ComIPdu_type ComIPdu[] = 
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
		.ComTxIPdu = NULL,
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
