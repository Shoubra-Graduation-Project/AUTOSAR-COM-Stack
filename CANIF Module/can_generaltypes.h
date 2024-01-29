/* 
 * File:   canif_Types.h
 * Author: DELL
 *
 * Created on December 1, 2023, 5:08 PM
 */

#ifndef CANIF_GENERAL_TYPES_H
#define	CANIF_GENERAL_TYPES_H

 /* Section : Includes */

///todo should this file be included here?
#include "canif_cfg.h"

 /* Section : Macros Definition */

#define NUM_OF_HRHS              10
#define SDU_LENGHT               8
#define CANID_EXPECTED           2

 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

/* CanIf_PduModeType */
typedef enum {
	CANIF_OFFLINE,
	CANIF_TX_OFFLINE,
	CANIF_TX_OFFLINE_ACTIVE,
    CANIF_ONLINE
} CanIf_PduModeType;


typedef enum {
	CAN_T_STOP = 1, // cannot request mode CAN_UNINIT
	CAN_T_START,
	CAN_T_SLEEP,
	CAN_T_WAKEUP
} Can_StateTransitionType;

typedef enum {
  /** UNINIT mode. Default mode of the CAN driver and all
   *  CAN controllers connected to one CAN network after
   *  power on. */
  CANIF_CS_UNINIT = 0,

  /**  STOPPED mode. At least one of all CAN controllers
   *   connected to one CAN network are halted and does
   *   not operate on the bus. */
  CANIF_CS_STOPPED,

  /** STARTED mode. All CAN controllers connected to
   *  one CAN network are started by the CAN driver and
   *  in full-operational mode. */
  CANIF_CS_STARTED,

  /** SLEEP mode. At least one of all CAN controllers
   *  connected to one CAN network are set into the
   *  SLEEP mode and can be woken up by request of the
   *  CAN driver or by a network event (must be supported
   *  by CAN hardware) */
  CANIF_CS_SLEEP
} CanIf_ControllerModeType;



typedef enum {
	CAN_OK,
	CAN_NOT_OK,
	CAN_BUSY,
 	CAN_WAKEUP,
} Can_ReturnType;

typedef enum {
	/** Transceiver mode NORMAL */
  CANTRCV_TRCVMODE_NORMAL = 0,
  /** Transceiver mode STANDBY */
  CANTRCV_TRCVMODE_STANDBY,
  /** Transceiver mode SLEEP */
  CANTRCV_TRCVMODE_SLEEP
} CanTrcv_TrcvModeType ;






typedef uint32 Can_IdType;
typedef uint16 PduIdType;
typedef uint16 PduLengthType;




typedef struct Can_PduType_s {
    // the CAN ID, 29 or 11-bit
    Can_IdType 	id;
    // Length, max 8 bytes
    uint8		length;
    // data ptr
    uint8*      sdu;
    // private data for CanIf,just save and use for callback
    PduIdType   swPduHandle;
} Can_PduType;



typedef struct
{
	uint8* SduDataPtr;
	uint8* MetaDataPtr;
	PduLengthType   SduLength;

} PduInfoType;


uint8 CANIF_NUM_TX_PDU_ID = 10;


typedef uint8 Can_HwHandleType;


/** SWS_CAN_00496 */
typedef struct
{
    Can_IdType CanId;       /* CAN ID of the CAN L-PDU */
    Can_HwHandleType Hoh;   /* ID of the corresponding hardware Object Range */
    uint8 ControllerId;     /* Cntrollre provided by Canif clearly identify the corresponding controller */

} Can_HwType;



typedef struct {
    /// can id used for transmission, msb indicates extended id
    Can_IdType id;

    /// data length (DLC)
    uint8 dlc;

    /// can driver controller id to be used for transmission
    uint8 controller;

    /// can driver hth id to be used for transmission
    Can_HwHandleType hth;

    /// upper layer confirmation function, set to null if no confirmation
    void(*user_TxConfirmation)(PduIdType txPduId);

    /// upper layer pdu id passed to callout function
    PduIdType ulPduId;
} CanIf_TxPduConfig;


typedef struct {
    /// can id used for reception filtering
    ///todo add support for range reception
    Can_IdType id;

    /// min dlc and dlc reported to upper layers. Set to -1 to disable dlc check
    uint8 dlc;

    /// can driver controller id from where to receive lpdu
    uint8 controller;

    /** SWS_CANIF_00012
     upper layer indication function, set to null if no rx indication */
    void(*user_RxIndication)(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

    /// upper layer pdu id passed to callout function
    PduIdType ulPduId;
} CanIf_RxPduConfig;


typedef struct
{
    union {
        PduIdType lpduId;
        PduIdType* array;
    }pduInfo;

    PduIdType arrayLen; // 0 means no ptr no filtering = fullCan reception (one single CanId) else ->>> (Range of IDs of group of single IDs)
}CanIf_HrHConfigType;

typedef struct {
    /* Everything in this structure is implementation specific */
    const CanIf_TxPduConfig* TxPduCfg;
    const CanIf_RxPduConfig* RxLpduCfg;
    const CanIf_HrHConfigType** canIfHrhCfg;  // This is an array of Hrh objects, for each controller ID
} CanIf_ConfigType;


extern const CanIf_ConfigType CanIf_Config;


 /* Section : Function Declaration */

#endif	/* CANIF_GENERAL_TYPES_H */

