/*******************************************************************************************************************************
FileName:                    							CanIf_Cfg.c                                     
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
/******************************************************************************************************************************
 **                                                     Includes                                                             **
 ******************************************************************************************************************************/
#include "../../CanDrv/Inc/Can.h"
/*****************************************************************************************************************************
 **                      					Post-Build Configuration variables values                                       **
 *****************************************************************************************************************************/
CanIf_ConfigType CanIf =
{
	.CanIfCtrlDrvCfg = 
	{
		.CanIfCtrlDrvInitHohConfigRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U],
		.CanIfCtrlCfg = 
		{
			{
				.CanIfCtrlCanCtrlRef = &Can.CanConfigSet.CanController[0U],
				.CanIfCtrlId = 0x0,
				.CanIfCtrlWakeupSupport = false
			}
		 }
	 },
	 
	.CanIfInitCfg =
	{
		.CanIfBufferCfg =
		{
			{
				.CanIfBufferHthRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U].CanIfHthCfg[0U],
				.CanIfBufferSize = 1
			}
		}, 
		.CanIfInitHohCfg = 
		{
			{
				.CanIfHthCfg =
				{
					{
						.CanIfHthCanCtrlIdRef = &CanIf.CanIfCtrlDrvCfg.CanIfCtrlCfg[0U],
						.CanIfHthIdSymRef = &Can.CanConfigSet.CanHardwareObject[0U]
					}
				},
				 
				.CanIfHrhCfg =
				{
					{
						.CanIfHrhCanCtrlIdRef = &CanIf.CanIfCtrlDrvCfg.CanIfCtrlCfg[0U],
						.CanIfHrhIdSymRef = &Can.CanConfigSet.CanHardwareObject[1U]
					}
				}
			}
		},
		 
		.CanIfRxPduCfg = 
		{
			{
				.CanIfRxPduCanId = 0x0,
				.CanIfRxPduCanIdMask = 0x0,
				.CanIfRxPduCanIdType = STANDARD_CAN,
				.CanIfRxPduDataLength = 40,
				.CanIfRxPduHrhIdRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U].CanIfHrhCfg[0U],
				.CanIfRxPduId = 1,
				.CanIfRxPduReadData = true,
				.CanIfRxPduReadNotifyStatus = false,
				.CanIfRxPduUserRxIndicationUL = COM,
				.CanIfRxPduCanIdRange = 
				{
					.CanIfRxPduCanIdRangeLowerCanId = 0,
					.CanIfRxPduCanIdRangeUpperCanId = 2000
				}
			}
		},
		 
		.CanIfTxPduCfg =
		{
			{
				.CanIfTxPduBufferRef = &CanIf.CanIfInitCfg.CanIfBufferCfg[0U],
				.CanIfTxPduCanId = 0x0,
				.CanIfTxPduCanIdType = STANDARD_CAN,
				.CanIfTxPduId = 0x0,
				.CanIfTxPduReadNotifyStatus = true,
				.CanIfTxPduType = STATIC,
				.CanIfTxPduUserTxConfirmationUL = COM
			}
		}
	}
};



const CanIf_TxPduConfigType CanIfTxPduConfigData[CANIF_NUM_TX_PDU_ID] =
{
  {

    .id = 512,
    .dlc = 8,
    .controller = 0,
    .hth = 0, //&CanIfHthConfigData_Hoh[0],
    .user_TxConfirmation = NULL,
    .ulPduId = 0
  },
  {
      .id = 0x13F, /* throttle position parameters */
      .dlc = 8,
      .controller = 0,
      .hth = 0,
      .user_TxConfirmation = NULL,
      .ulPduId = 1
  },
};

const CanIf_RxLPduConfigType CanIfRxPduConfigData[CANIF_NUM_RX_LPDU_ID] =
{ /* CAN ID must be sorted from high to low! */
    { /* Contains throttle body position sensor, and some temperature values */
        .id = 0x318,
        .dlc = 8,
        .controller = 0,
        .user_RxIndication = &IMACanRxIndication,
        .ulPduId = 0
    },
    { /* contains the SOC */
        .id = 0x231,
        .dlc = 7,
        .controller = 0,
        .user_RxIndication = &IMACanRxIndication,
        .ulPduId = 1
    },
    { /* contains the motor current */
        .id = 0x169,
        .dlc = 8,
        .controller = 0,
        .user_RxIndication = &IMACanRxIndication,
        .ulPduId = 2
    },
    {
        .id = 0x111, /* contains engine RPM, and battery voltage */
        .dlc = 7,
        .controller = 0,
        .user_RxIndication = &IMACanRxIndication,
        .ulPduId = 3
    },



};


// This container includes all necessary configuration sub-containers
// according the CAN Interface configuration structure.
const CanIf_ConfigType CanIf_Config =
{
  .ControllerConfig = CanIfControllerConfig,
  .DispatchConfig = &CanIfDispatchConfig,
  .TxPduCfg = CanIfTxPduConfigData,
  .RxLpduCfg = CanIfRxPduConfigData,
  .canIfHrhCfg = CanIfHrhConfigData_Hoh,
  //.InitConfig = &CanIfInitConfig,
  //.TransceiverConfig = NULL, // Not used
  //.Arc_ChannelToControllerMap = CanIf_Arc_ChannelToControllerMap,
  //.Arc_ChannelDefaultConfIndex = CanIf_Arc_ChannelDefaultConfIndex,
};

const CanIf_PduModeType pdu_mode = {
	CANIF_OFFLINE,
};

const CanIf_ControllerModeType controller_mode = {
	CANIF_CS_UNINIT,
};

const CanIf_Channel_t canif_channel = {
	CANIF_CHANNEL_1,
};
