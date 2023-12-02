 /* Section : Includes */
#include "canif.h"

 /* Section : Macros Definition */

 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

 /* Section : Function Declaration */


//void CanIf_Init(const CanIf_ConfigType* ConfigPtr){}

STD_ReturnType CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

STD_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

//STD_ReturnType CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr){}

//void CanIf_RxIndication(Can_HwHandleType hrh, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr, uint8 driverUnit){}

STD_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest){
    STD_ReturnType RET = E_OK;
    
    return RET;
}
STD_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduGetModeType* PduModePtr){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

STD_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode ){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

STD_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr ){
    STD_ReturnType RET = E_OK;
    
    return RET;
}