/* 
 * File:   canif.h
 * Author: DELL
 *
 * Created on December 1, 2023, 5:08 PM
 */

#ifndef CANIF_H
#define	CANIF_H

 /* Section : Includes */
#include "std_libraries.h"
#include "std_types.h"
#include "canif/canif_types.h"

 /* Section : Macros Definition */

 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

 /* Section : Function Declaration */


//void CanIf_Init(const CanIf_ConfigType* ConfigPtr);

STD_ReturnType CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode);

STD_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr);


STD_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

void CanIf_RxIndication(const Can_HwType* MailBox, const PduInfoType* PduInfoPtr);

STD_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest);
STD_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduGetModeType* PduModePtr);

STD_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode );
STD_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr );



#endif	/* CANIF_H */

