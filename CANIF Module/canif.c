 /* Section : Includes */
#include "canif.h"

 /* Section : Macros Definition */

 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

 /* Section : Function Declaration */

static const CanIf_ConfigType* CanIf_ConfigPtr;




Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) 
{

    Std_ReturnType RET = E_OK;
    Can_PduType CanPdu;
    CanIf_ControllerModeType ControllerMode = (CanIf_ControllerModeType)0;
    CanIf_PduModeType PduMode = (CanIf_PduModeType)0;

    //Check CAN is INITIATE or Not
    if (CanIfState != CANIF_INIT) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_UNINIT);
        return E_NOT_OK;
    }

    //Check pointer != Null
    if (PduInfoPtr == NULL) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    //Check ID if it's Valid unexceed Range
    if (TxPduId < CANIF_NUM_TX_PDU_ID) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_INVALID_TXPDUID);
        return E_NOT_OK;
    }

    //Check Controller Mode
    if (CanIf_GetControllerMode(CanIf_ConfigPtr->TxPduCfg[TxPduId].controller, &ControllerMode) != E_OK) {
        return E_NOT_OK;
    }

    //Check PDU Mode
    if (CanIf_GetPduMode(CanIf_ConfigPtr->TxPduCfg[TxPduId].controller, &PduMode) != E_OK) {
        return E_NOT_OK;
    }

    // channel not started, report to Det and return
    if (ControllerMode != CAN_CS_STARTED) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_CTRLMODE);
        return E_NOT_OK;
    }

    // TX is not online, report to Det and return
    if (PduMode != CANIF_ONLINE && PduMode != CANIF_TX_OFFLINE_ACTIVE) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_PDU_MODE);
        return E_NOT_OK;
    }
    Can_IdType canId;
    canId = CanIf_ConfigPtr->TxPduCfg[TxPduId].id;

    CanPdu.length = PduInfoPtr->SduLength;
    CanPdu.id = canId;
    CanPdu.swPduHandle = TxPduId;
    CanPdu.sdu = PduInfoPtr->SduDataPtr;

    Can_HwHandleType hth = CanIf_ConfigPtr->TxPduCfg[TxPduId].hth;


    //RET = Can_Write(hth, &CanPdu);

    return RET;
}


static void RxLPduReceived(PduIdType lpdu, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr)
{
    CanIf_PduModeType PduMode = (CanIf_PduModeType)0;

    //Check PDU Mode
    if (CanIf_GetPduMode(CanIf_ConfigPtr->RxLpduCfg[lpdu].controller, &PduMode) != E_OK) {
        return E_NOT_OK;
    }

    // RX is not online, report to Det and return
    if (PduMode == CANIF_OFFLINE) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_PDU_MODE);
        // Rx not online,discard message.
        return E_NOT_OK;
    }

    // call eventual callback
    if (CanIf_ConfigPtr->RxLpduCfg[lpdu].user_RxIndication) {
        PduInfoType pduInfo = {
          .SduLength = canDlc,
          .SduDataPtr = (uint8*)canSduPtr,
        };
        (*CanIf_ConfigPtr->RxLpduCfg[lpdu].user_RxIndication)(CanIf_ConfigPtr->RxLpduCfg[lpdu].ulPduId, &pduInfo);
    }
}



void CanIf_RxIndication(const Can_HwType* MailBox, const PduInfoType* PduInfoPtr) 
{





}



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