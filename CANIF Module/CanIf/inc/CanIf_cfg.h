/* 
 * File:   canif_cfg.h
 * Author: DELL
 *
 * Created on December 1, 2023, 5:54 PM
 */

#ifndef CANIF_CFG_H
#define	CANIF_CFG_H

/// number of hths referenced by canif
#define CANIF_NUM_HTHS                            NUM_OF_HTHS
/// number of driver units
///todo change this name to something from specificatoin
#define CANIF_NUM_DRIVER_UNITS                    1

/// Enabled version info
#define CANIF_PUBLIC_VERSION_INFO_API             STD_ON
/// Enables the  CanIf_ReadRxPduData function
#define CANIF_PUBLIC_READRXPDU_DATA_API		STD_ON
/// Enables the CanIf_CancelTransmit function
#define CANIF_CANCEL_TRANSMIT_SUPPORT		STD_ON
/// Enables the CanIf_ReadTxNotifStatus function
#define CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API	STD_ON
/// Enables the CanIf_ReadRxNotifStatus function
#define CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API	STD_ON
/// Enables the CanIf_GetTxConfirmationState function
#define CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT    STD_ON
/// Enables the CanIf_SetDynamicTxId function
#define CANIF_PUBLIC_SETDYNAMICTXID_API            STD_ON
/// Enabled the CanIf_CancelTxConfirmation function
#define CANIF_CTRLDRV_TX_CANCELLATION				      STD_ON
/// Enables the dlc check for received frames
#define CANIF_PRIVATE_DLC_CHECK                   STD_ON
/// Selects type of software filtering, only binary search supported
#define CANIF_PRIVATE_SOFTWARE_FILTER_TYPE			  BINARY
/// Datatype for hoh ids, uint8 or uint16
#define CANIF_PUBLIC_HANDLE_TYPE_ENUM				      uint16
/// Enables the tx buffering mechanism
#define CANIF_PUBLIC_TX_BUFFERING					        STD_ON
/// Enables use of multiple can driver units
#define CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT			    STD_OFF // Not supported
/// Enables DET error reports
#define CANIF_DEV_ERROR_DETECT			              STD_OFF
/// Enabled the CanIf_CheckValidation function
#define CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_API  STD_OFF // not supported
/// Defines if a can controller has wakeup support
#define CANIF_CTRL_WAKEUP_SUPPORT                 STD_OFF // not supported
/// Defines if a can tranceiver has wakeup support
#define CANIF_TRCV_WAKEUP_SUPPORT                 STD_OFF // not supported
/**************************************************************************************************
**
Name:                                     CanIfMaxRxPduCfg

Type:                                   EcucIntegerParamDef

Description:  			Maximum number of Pdus. This parameter is needed only in case of
						post-build loadable implementation using static memory allocation.		**
**************************************************************************************************/
#define CanIfMaxRxPduCfg	(uint32)1

/**************************************************************************************************
**
Name:                                    CanIfMaxTxPduCfg

Type:                                   EcucIntegerParamDef

Description:  			Maximum number of Pdus. This parameter is needed only in case of
						post-build loadable implementation using static memory allocation.		**
**************************************************************************************************/
#define CanIfMaxTxPduCfg	(uint32)1

/**************************************************************************************************
**
Name:                              CanIfPublicReadRxPduNotifyStatusApi

Type:                                    EcucBooleanParamDef

Description: 				Enables and disables the API for reading the notification status of
											receive L-PDUs.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicReadRxPduNotifyStatusApi	false

/**************************************************************************************************
**
Name:                              CanIfPublicReadTxPduNotifyStatusApi

Type:                                    EcucBooleanParamDef

Description: 				Enables and disables the API for reading the notification status of
											transmit L-PDUs.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicReadTxPduNotifyStatusApi	false

#endif	/* CANIF_CFG_H */

