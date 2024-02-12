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
#include "Platform_Types.h"

 /* Section : Macros Definition */

#define NUM_OF_HRHS              1
#define NUM_OF_HOHS              1
#define NUM_OF_HTHS              1

 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

typedef uint8 Can_HwHandleType;


typedef struct 
{
    // the CAN ID, 29 or 11-bit
    Can_IdType 	id;
    // Length, max 8 bytes
    uint8       length;
    // data ptr
    uint8*      sdu;
    // Controller Id
    PduIdType  swPduHandle;
} Can_PduType;


/** SWS_CAN_00496 */
typedef struct
{
    Can_IdType CanId;       /* CAN ID of the CAN L-PDU */
    Can_HwHandleType HOH;   /* ID of the corresponding hardware Object Range */
    uint8 ControllerId;     /* Cntroller provided by Canif clearly identify the corresponding controller */

} Can_HwType;

 /* Section : Function Declaration */

#endif	/* CANIF_GENERAL_TYPES_H */
