 /* Section : Includes */
#include "canif.h"

 /* Section : Macros Definition */

 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

static const CanIf_ConfigType* CanIf_ConfigPtr;
static CanIf_LPduDataType lPduData;

 /* Section : Function Declaration */


Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) 
{

    Std_ReturnType RET = E_OK;
    Can_PduType CanPdu;
    CanIf_ControllerModeType ControllerMode = (CanIf_ControllerModeType)0;
    CanIf_PduModeType PduMode = (CanIf_PduModeType)0;

    //Check CAN is INITIATE or Not
    if (CanIfState != CANIF_INIT) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_ID, CANIF_E_UNINIT);
        return E_NOT_OK;
    }

    //Check pointer != Null
    /* SWS_CANIF_00320 */
    if (PduInfoPtr == NULL) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKVALIDATION_ID, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    //Check DataLength not exceeed maximum
    /* SWS_CANIF_00893 */
    if ((PduInfoPtr->SduLength) > SDU_LENGTH) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKVALIDATION_ID, CANIF_E_TXPDU_LENGTH_EXCEEDED);
        return E_NOT_OK;
    }

    //Check ID if it's Valid unexceed Range
    /* SWS_CANIF_00319 */
    if (TxPduId > CANIF_NUM_TX_PDU_ID) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_INVALID_TXPDUID);
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
    /* SWS_CANIF_00317 */
    if (ControllerMode != CAN_CS_STARTED) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_CTRLMODE);
        return E_NOT_OK;
    }

    // TX is not online, report to Det and return
    if (PduMode != CANIF_ONLINE) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_PARAM_PDU_MODE);
        return E_NOT_OK;
    }

    //Copy data to CanPdu
    Can_IdType canId;
    canId = CanIf_ConfigPtr->TxPduCfg[TxPduId].id;

    CanPdu.length = PduInfoPtr->SduLength;
    CanPdu.id = canId;
    CanPdu.sdu = PduInfoPtr->SduDataPtr;
    CanPdu.controllerId = CanIf_ConfigPtr->TxPduCfg[TxPduId].controller;

    Can_HwHandleType hth = CanIf_ConfigPtr->TxPduCfg[TxPduId].hth;

    /* SWS_CANIF_00162 */
    //RET = Can_Write(hth, &CanPdu);

    return RET;
}




Std_ReturnType CanIf_RxIndication(const Can_HwType* MailBox, const PduInfoType* PduInfoPtr) 
{
    Std_ReturnType RET = E_OK;
    CanIf_PduModeType PduMode = (CanIf_PduModeType)0;
 
    //Check CAN is INITIATE or Not
    if (CanIfState != CANIF_INIT) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_ID, CANIF_E_UNINIT);
        return E_NOT_OK;
    }

    //Check CAN is INITIATE or Not & Check pointer != Null & MailBox
    /* SWS_CANIF_00419 */
    if ((MailBox == NULL) || (PduInfoPtr == NULL) || ((PduInfoPtr->SduDataPtr) == NULL)) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKVALIDATION_ID, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
    }


    // Check if MailBox->Hoh has Invalid Value
    /* SWS_CANIF_00416 */
    if ((MailBox->Hoh) > NUM_OF_HOHS) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_ID, CANIF_E_PARAM_HRH);
        return E_NOT_OK;
    }
  
    // Check Range of IDs
    /* SWS_CANIF_00417 */
    if (((MailBox->CanId) < CANID_EXPECTED_MIN) && ((MailBox->CanId) < CANID_EXPECTED_MAX)) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_ID, CANIF_E_PARAM_CANID);
        return E_NOT_OK;
    }

    // Check if Data Lenght has invalid Value
    /* SWS_CANIF_00417 */
    if ((PduInfoPtr->SduLength) > SDU_LENGTH) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_ID, CANIF_E_PARAM_DLC);
        return E_NOT_OK;
    }

    //Check PDU Mode
    if (CanIf_GetPduMode(CanIf_ConfigPtr->RxLpduCfg[lpdu].controller, &PduMode) != E_OK) {
        return E_NOT_OK;
    }

    // RX is not online and tx offline active not, report to Det and return
    if (PduMode != CANIF_ONLINE && PduMode != CANIF_TX_OFFLINE_ACTIVE) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_PARAM_PDU_MODE);
        // Rx not online,discard message.
        return E_NOT_OK;
    }

    /* ------------------------------------ Filteraing ------------------------------------ */
    int numberofPdus = CanIf_ConfigPtr->canIfHrhCfg[MailBox->ControllerId][MailBox->Hoh].arrayLen;

    // There's 1 Pdu only so go on
    if (numberofPdus == 0) {
        PduIdType *PduId = CanIf_ConfigPtr->canIfHrhCfg[MailBox->ControllerId][MailBox->Hoh].pduInfo.lpduId;
        // no filtering, lpdu id found
        lPduData.rxLpdu[PduId].dlc = PduInfoPtr->SduLength;
        memcpy(lPduData.rxLpdu[PduId].data, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

        // call eventual callback
        (*CanIf_ConfigPtr->RxLpduCfg[PduId].user_RxIndication)(CanIf_ConfigPtr->RxLpduCfg[PduId].ulPduId, &PduInfoPtr);
    }
    else {
        // Get first pduid to go on if there's multiple pduid
        PduIdType *PduId = CanIf_ConfigPtr->canIfHrhCfg[MailBox->ControllerId][MailBox->Hoh].pduInfo.array;
        while (numberofPdus > 1) {
            if (CanIf_ConfigPtr->RxLpduCfg[PduId[numberofPdus / 2]].id >= MailBox->CanId) {
                PduId += numberofPdus / 2;
                numberofPdus = numberofPdus / 2 + numberofPdus % 2;
            }
            else  numberofPdus = numberofPdus / 2;
        }
        if (CanIf_ConfigPtr->RxLpduCfg[*PduId].id == MailBox->CanId) {
            // lpdu id found
            lPduData.rxLpdu[*PduId].dlc = PduInfoPtr->SduLength;
            memcpy(lPduData.rxLpdu[*PduId].data, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

            // call eventual callback
            (*CanIf_ConfigPtr->RxLpduCfg[*PduId].user_RxIndication)(CanIf_ConfigPtr->RxLpduCfg[*PduId].ulPduId, &PduInfoPtr); 
        }
    }
    return RET;
}


Std_ReturnType CanIf_ReadRxPduData(PduIdType  CanIfRxSduId, PduInfoType* CanIfRxInfoPtr)
{
    Std_ReturnType RET = E_OK;
    CanIf_ControllerModeType ControllerMode = (CanIf_ControllerModeType)0;

    //Check CAN is INITIATE or Not
    if (CanIfState != CANIF_INIT) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_ID, CANIF_E_UNINIT);
        return E_NOT_OK;
    }

    //Check pointer != Null
    /* SWS_CANIF_00326 */
    if (CanIfRxInfoPtr == NULL) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKVALIDATION_ID, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    //Check Validation of CanIfRxSduId
    /* SWS_CANIF_00325 */
    if (CanIfRxSduId > CANIF_NUM_RX_LPDU_ID) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXNOTIFSTATUS_ID, CANIF_E_INVALID_RXPDUID);
        return E_NOT_OK;
    }

    //Check Controller Mode
    if (CanIf_GetControllerMode(CanIf_ConfigPtr->TxPduCfg[TxPduId].controller, &ControllerMode) != E_OK) {
        return E_NOT_OK;
    }

    // channel not started, report to Det and return
    /* SWS_CANIF_00324 */
    if (ControllerMode != CAN_CS_STARTED) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_CTRLMODE);
        return E_NOT_OK;
    }

    // Copy Data
    uint8 dlc = lPduData.rxLpdu[CanIfRxSduId].dlc;
    CanIfRxInfoPtr->SduLength = dlc;
    memcpy(CanIfRxInfoPtr->SduDataPtr, lPduData.rxLpdu[CanIfRxSduId].data, dlc);

    return RET;
}


Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr){
    STD_ReturnType RET = E_OK;
    
    return RET;
}


Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest){
    STD_ReturnType RET = E_OK;
    
    return RET;
}
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduGetModeType* PduModePtr){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

Std_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode ){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

Std_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr ){
    STD_ReturnType RET = E_OK;
    
    return RET;
}
