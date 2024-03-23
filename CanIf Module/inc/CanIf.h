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
#include "../COM module/include/ComStack_Types.h"
#include "./CanIf_types.h"
#include "./CanIf_cfg.h"

#if defined(USE_PDUR)
#include "PduR.h"
#endif

#if defined(USE_COM)
#include "../COM module/include/Com.h"
#endif

 /* Section : Macros Definition */
#define CANIF_VENDOR_ID          (VENDOR_ID_ARCCORE)
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
CanIf_ControllerModeType CurrentCanState[NUMBER_OF_CONTROLLERS];

 /* Section : Function Declaration */
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode);
Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr);
Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest);
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr);
void CanIf_Init(const CanIf_ConfigType* ConfigPtr);
void CanIf_TxConfirmation (PduIdType CanTxPduId);
Std_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode );
Std_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr );
Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
Std_ReturnType CanIf_RxIndication(const Can_HwType* MailBox, const PduInfoType* PduInfoPtr);
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr);
#endif	/* CANIF_H */
