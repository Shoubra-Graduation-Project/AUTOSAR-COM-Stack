/* 
 * File:   canif_types.h
 * Author: DELL
 *
 * Created on December 1, 2023, 6:00 PM
 */

#ifndef CANIF_TYPES_H
#define	CANIF_TYPES_H

#include "../can_generaltypes.h"

/** Status of the PDU channel group. Current mode of the channel defines its
 *  transmit or receive activity. Communication direction (transmission and/or
 *  reception) of the channel can be controlled separately or together by upper
 *  layers. */
// value is bitcoded: bit 0 = rx online, bit 1 = tx online, bit 2 = tx notification online, byte 1 is mask for affected bits
typedef enum {
	/** Channel shall be set to the offline mode
	 *  => no transmission and reception */
	CANIF_SET_OFFLINE = 0x700,
	
	/** Receive path of the corresponding channel
	 *  shall be disabled */
	CANIF_SET_RX_OFFLINE = 0x100,

	/** Receive path of the corresponding channel
	 *  shall be enabled */
	CANIF_SET_RX_ONLINE = 0x101,
	
	/** Transmit path of the corresponding channel
	 *  shall be disabled */
	CANIF_SET_TX_OFFLINE = 0x600,
	
	/** Transmit path of the corresponding channel
	 *  shall be enabled */
	CANIF_SET_TX_ONLINE = 0x606,
	
	/** Channel shall be set to online mode
	 *  => full operation mode */
	CANIF_SET_ONLINE = 0x707,
	
	/** Transmit path of the corresponding channel
	 *  shall be set to the offline active mode
	 *  => notifications are processed but transmit
	 *  requests are blocked. */
	CANIF_SET_TX_OFFLINE_ACTIVE = 0x604
} CanIf_PduSetModeType;

// value is bitcoded: bit 0 = rx online, bit 1 = tx online, bit 2 = tx notification online
typedef enum {
	/** Channel is in the offline mode ==> no transmission or reception */
  CANIF_GET_OFFLINE = 0,
  /** Receive path of the corresponding channel is enabled and
   *  transmit path is disabled */
  CANIF_GET_RX_ONLINE = 0x1,
  /** Transmit path of the corresponding channel is enabled and
   *  receive path is disabled */
  CANIF_GET_TX_ONLINE = 0x6,
  /** Channel is in the online mode ==> full operation mode */
  CANIF_GET_ONLINE = 0x7,
  /** Transmit path of the corresponding channel is in
   *  the offline mode ==> transmit notifications are processed but
   *  transmit requests are blocked. The receiver path is disabled. */
  CANIF_GET_OFFLINE_ACTIVE = 0x4,
  /** Transmit path of the corresponding channel is in the offline
   *  active mode ==> transmit notifications are processed but transmit
   *  requests are blocked. The receive path is enabled. */
  CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE = 0x5
	
} CanIf_PduGetModeType;

typedef enum {
	/** No transmit or receive event occurred for
	 *  the requested L-PDU. */
	CANIF_NO_NOTIFICATION = 0,	
	/** The requested Rx/Tx CAN L-PDU was
	 *  successfully transmitted or received. */
	CANIF_TX_RX_NOTIFICATION
	
} CanIf_NotifStatusType;



#endif	/* CANIF_TYPES_H */

