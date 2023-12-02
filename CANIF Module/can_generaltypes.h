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




 /* Section : Function Declaration */

#endif	/* CANIF_GENERAL_TYPES_H */

