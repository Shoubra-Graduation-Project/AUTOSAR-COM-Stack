/* 
 * File:   canif.h
 * Author: 
 *
 * Created on December 1, 2023, 5:08 PM
 */

#ifndef CANIF_H
#define	CANIF_H

 /* Section : Includes */
#include "../../Common/Std_Types.h"

#include "./CanIf_types.h"

 /* Section : Macros Definition */
#define CANIF_VENDOR_ID          (VENDOR_ID_ARCCORE)
#define CANIF_MODULE_ID          (1)
#define CANIF_AR_MAJOR_VERSION   3
#define CANIF_AR_MINOR_VERSION   1
#define CANIF_AR_PATCH_VERSION   5

#define CANIF_SW_MAJOR_VERSION   1
#define CANIF_SW_MINOR_VERSION   3
#define CANIF_SW_PATCH_VERSION   0

#define  NUMBER_OF_CONTROLLERS        (uint8)2

#if defined(USE_DET)
#include "Det.h"
#endif
 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

 /* Section : Function Declaration */
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode);
Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr);
Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest);
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr);
void CanIf_Init(const CanIf_ConfigType* ConfigPtr);
void CanIf_TxConfirmation (PduIdType CanTxPduId);
Std_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode );
Std_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr );
#endif	/* CANIF_H */
