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
