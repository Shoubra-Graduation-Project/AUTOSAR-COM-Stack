#ifndef CANIF_H_
#define CANIF_H_


#include "Std_Types.h"

#define CANIF_VENDOR_ID          (VENDOR_ID_ARCCORE)
#define CANIF_MODULE_ID          (MODULE_ID_CANIF)
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

#if defined(USE_COM)
#include "Com.h"
#endif

#include "Canif_Types.h"
#include "CanIf_Cfg.h"
#include "Compiler.h"
#include "Modules.h"







void CanIf_Init(const CanIf_ConfigType* ConfigPtr);
FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode);
FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, P2VAR(Can_ControllerStateType,CANIF_CODE,AUTOMATIC) ControllerModePtr);

