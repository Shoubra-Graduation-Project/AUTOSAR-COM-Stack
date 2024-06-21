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



typedef enum
{
	CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS = 1
} CanIf_DynamicTxLPduIdType;


typedef enum {
  CANIF_PDU_ID_PDU_UDSphysicalTX = CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS,
  CANIF_PDU_ID_PDU_TestData,
  CANIF_NUM_TX_PDU_ID
} CanIf_StaticTxLPduIdType;

typedef enum
{
    CANIF_PDU_ID_PDU_IMA_ID111,
    CANIF_PDU_ID_PDU_IMA_ID169,
    CANIF_PDU_ID_PDU_IMA_ID231,
    CANIF_PDU_ID_PDU_IMA_ID318,
    CANIF_NUM_RX_LPDU_ID
} CanIf_RxLPduIdType;
/**************************************************************************************************
**
Name:                                   CanIfPrivateDlcCheck

Type:                                    EcucBooleanParamDef

Description: 				Selects whether the DLC check is supported.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPrivateDlcCheck	false
/**************************************************************************************************
**
Name:                                CanIfPrivateSoftwareFilterType

Type:                                    EcucEnumerationParamDef

Description: 				Selects the desired software filter mechanism for reception only.
							Each implemented software filtering method is identified by this enumeration number.

Range: 								Types implemented software filtering methods
									BINARY Selects Binary Filter method.
									INDEX Selects Index Filter method.
									LINEAR Selects Linear Filter method.
									TABLE Selects Table Filter method.							**
**************************************************************************************************/
#define CanIfPrivateSoftwareFilter	(CanIfPrivateSoftwareFilterType)LINEAR
/***************************************************************************************************/


/**************************************************************************************************
**
Name:                                   CanIfPublicCfg
Type:                                      Structure
Description:  			This container contains the public configuration (parameters) of the
										CAN Interface.											**
**************************************************************************************************/
/**************************************************************************************************
**
Name:                                   CanIfPublicDevErrorDetect

Type:                                    EcucBooleanParamDef

Description: 				Switches the Default Error Tracer (Det) detection and notification ON or OFF.
										• true: enabled (ON).
										• false: disabled (OFF).								**
**************************************************************************************************/
#define CanIfPublicDevErrorDetect false
/**************************************************************************************************
**
Name:                                   CanIfPublicHandleTypeEnum

Type:                                    EcucEnumerationParamDef

Description: 				This parameter is used to configure the Can_HwHandleType. The
							Can_HwHandleType represents the hardware object handles of a CAN
							hardware unit. For CAN hardware units with more than 255 HW objects
							the extended range shall be used (UINT16).							**
**************************************************************************************************/
#define CanIfPublicHandleTypeEnum	uint8
/**************************************************************************************************
**
Name:                                 CanIfPublicReadRxPduDataApi

Type:                                    EcucBooleanParamDef

Description: 				Enables / Disables the API CanIf_ReadRxPduData() for reading
										received L-SDU data.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicReadRxPduDataApi	true
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
/**************************************************************************************************
**
Name:                              		CanIfPublicTxBuffering

Type:                                    EcucBooleanParamDef

Description: 				Enables and disables the buffering of transmit L-PDUs (rejected by the
								CanDrv) within the CAN Interface module.	
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicTxBuffering	false
/**************************************************************************************************
**
Name:                              CanIfPublicTxConfirmPollingSupport

Type:                                    EcucBooleanParamDef

Description: 				Configuration parameter to enable/disable the API to poll for Tx
										Confirmation state.										**
**************************************************************************************************/
#define CanIfPublicTxConfirmPollingSupport false
/**************************************************************************************************
**
Name:                              		CanIfSetBaudrateApi

Type:                                   EcucBooleanParamDef

Description: 				Configuration parameter to enable/disable the CanIf_SetBaudrate API
							to change the baud rate of a CAN Controller. If this parameter is set to
							true the CanIf_SetBaudrate API shall be supported. Otherwise the API
							is not supported.													**
**************************************************************************************************/
#define CanIfSetBaudrateApi true
/*************************************************************************************************/

/**************************************************************************************************
**
Name:                                     CanIfInitCfg
Type:                                      Structure
Description:  			This container contains the init parameters of the CAN Interface.		**
**************************************************************************************************/
/**************************************************************************************************
**
Name:                                   CanIfMaxBufferSize

Type:                                   EcucIntegerParamDef

Description:  			Maximum total size of all Tx buffers. This parameter is needed only in
						case of post-build loadable implementation using static memory
											allocation.											**
**************************************************************************************************/
#define CanIfMaxBufferSize	(uint32)10
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

#endif	/* CANIF_CFG_H */

