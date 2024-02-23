 /******************************************************************************
 *
 * Module: Common - Macros
 *
 * File Name: Common_Macros.h
 *
 * Description: Commonly used Macros
 *
 * Author: 
 *
 *******************************************************************************/

#ifndef COMMON_MACROS
#define COMMON_MACROS

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

/* Rotate right the register value with specific number of rotates */
#define ROR(REG,num) ( REG = (REG>>num) | (REG << ((sizeof(REG) * 8)-num)) )

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,num) ( REG = (REG<<num) | (REG >> ((sizeof(REG) * 8)-num)) )

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )
   
   
   
/**Macros Canif_types.h*/   
   
#define EXTENDED_CAN                              0
#define EXTENDED_FD_CAN                           1
#define STANDARD_CAN                              2
#define STANDARD_FD_CAN                           3


/** Channel is in the offline mode ==> no transmission or reception */
#define CANIF_GET_OFFLINE                         0
/** Receive path of the corresponding channel is enabled and
*transmit path is disabled */
#define CANIF_GET_RX_ONLINE                       1
/** Transmit path of the corresponding channel is enabled and
*receive path is disabled */
#define CANIF_GET_TX_ONLINE                       2
/** Channel is in the online mode ==> full operation mode */
#define CANIF_GET_ONLINE                          3
/** Transmit path of the corresponding channel is in
*the offline mode ==> transmit notifications are processed but
*transmit requests are blocked. The receiver path is disabled. */
#define CANIF_GET_OFFLINE_ACTIVE                  4
/** Transmit path of the corresponding channel is in the offline
*active mode ==> transmit notifications are processed but transmit
*requests are blocked. The receive path is enabled. */
#define CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE        5   
   
   
   
   
 /**canif.c*/ 
   
#define CANIF_CODE                           1  
#define CANIF_MODULE_ID                      (uint16_t)MODULE_ID_CANIF


/* Det defines */

#define CANIF_E_PARAM_CANID                 (uint8_t) 10
#define CANIF_E_PARAM_HOH                   (uint8_t) 12
#define CANIF_E_PARAM_LPDU                  (uint8_t) 13
#define CANIF_E_PARAM_CONTROLLERID          (uint8_t) 15
#define CANIF_E_PARAM_WAKEUPSOURCE          (uint8_t) 16
#define CANIF_E_PARAM_TRCV                  (uint8_t) 17
#define CANIF_E_PARAM_TRCVMODE              (uint8_t) 18
#define CANIF_E_PARAM_TRCVWAKEUPMODE        (uint8_t) 19
#define CANIF_E_PARAM_CTRLMODE              (uint8_t) 21
#define CANIF_E_PARAM_PDU_MODE              (uint8_t) 22
#define CANIF_E_PARAM_POINTER               (uint8_t) 20
#define CANIF_E_UNINIT                      (uint8_t) 30
#define CANIF_E_INVALID_TXPDUID             (uint8_t) 50
#define CANIF_E_INVALID_RXPDUID             (uint8_t) 60
#define CANIF_E_INIT_FAILED                 (uint8_t) 80
#define CANIF_E_INVALID_DATA_LENGTH         (uint8_t) 61   
   
   
   
   
#define CANIF_INIT_ID                        (uint8_t)0x01
#define CANIF_INIT_CONTROLLER_ID             (uint8_t)0x02
#define CANIF_SET_CONTROLLER_MODE_ID         (uint8_t)0x03
#define CANIF_GET_CONTROLLER_MODE_ID         (uint8_t)0x04
#define CANIF_GET_CONTROLLER_ERROR_STATE_ID  (uint8_t)0x4B
#define CANIF_TRANSMIT_ID                    (uint8_t)0x05
#define CANIF_READTXPDUDATA_ID               (uint8_t)0x06
#define CANIF_READTXNOTIFSTATUS_ID           (uint8_t)0x07
#define CANIF_READRXNOTIFSTATUS_ID           (uint8_t)0x08
#define CANIF_SET_PDU_MODE_ID                (uint8_t)0x09
#define CANIF_GET_PDU_MODE_ID                (uint8_t)0x0A
#define CANIF_CHECKVALIDATION_ID             (uint8_t)0x12
#define CANIF_TX_CONFIRMATION_ID             (uint8_t)0x13
#define CANIF_RX_INDICATION_ID               (uint8_t)0x14
#define CANIF_CONTROLLER_BUS_OFF_ID          (uint8_t)0x16

#define MAX_DATA_SIZE_FD                     (uint8_t)64
#define MAX_DATA_SIZE_CLASSIC               (uint8_t)8

#define IS_NOT_VALID_DATA_LENGTH(Datalength,FrameType)  ( ( ( FrameType == STANDARD_FD_CAN || FrameType == EXTENDED_FD_CAN) && Datalength > MAX_DATA_SIZE_FD )\
                        || ( ( FrameType == EXTENDED_CAN    || FrameType == STANDARD_CAN )    && Datalength > MAX_DATA_SIZE_CLASSIC) )



#define  CAN_GENERAL_CAN_DEV_ERROR_DETECT                   STD_ON

   
   
   
   
   
   

#endif
