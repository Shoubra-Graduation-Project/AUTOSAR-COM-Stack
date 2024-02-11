#include "../inc/CanIf.h"
#include "../inc/CanIf_cfg.h"
#include "../inc/CanIf_types.h"
#include "../../Det/inc/Det.h"
#include "../../Common/Compiler.h"



#if(CanIfPublicReadTxPduNotifyStatusApi==true)
    CanIf_NotifStatusType TxPduState[CanIfMaxTxPduCfg];
#endif

static enum CanIfStateType
{
    CANIF_UNINIT, CANIF_READY
} CanIfState = CANIF_UNINIT;

static CanIf_ControllerModeType CanIfControllerMode[NUMBER_OF_CONTROLLERS] = {
        CANIF_CS_UNINIT, CANIF_CS_UNINIT };

static CanIf_PduModeType CanIfPduMode[NUMBER_OF_CONTROLLERS];

static const CanIf_ConfigType* CanIf_ConfigPtr;
static CanIf_LPduDataType lPduData;

/******************************************* CanIf_SetControllerMode ***********************************************/
LOCAL VAR(CanIf_GlobalType ,AUTOMATIC) CanIf_Global;

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode) {
  
    VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType     = E_OK                          ;
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus         = E_NOT_OK                          ;
    VAR(CanIf_Channel_t ,AUTOMATIC ) channel_CanIf_SetControllerMode = (CanIf_Channel_t) ControllerId;
    
  
  /*check if INIT function is run */
    
      if (CanIfState == CANIF_UNINIT)
    {
	Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CTRLMODE);    
        return E_NOT_OK;
	   
    }
    else
    {
        /*MISRA*/
    }
  
    if(channel_CanIf_SetControllerMode >= MAX_NUM_CHANNELS)
    {
        /*[SWS_CANIF_00311] If parameter ControllerId of CanIf_SetControllerMode() has an invalid value,
		the CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError
		service of the DET module, when CanIf_SetControllerMode() is called.*/
      
        ErrorStatus            = E_OK                   ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        
        CanDevolpmentErrorType = CANIF_E_PARAM_CONTROLLERID ;
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CONTROLLERID);
#endif	 
    }
    else
    {
        /*MISRA*/
    }
  
  
    if ( ControllerMode != CANIF_CS_STARTED || ControllerMode != CANIF_CS_SLEEP || ControllerMode != CANIF_CS_STOPPED)
    {
        /* [SWS_CANIF_00774] If parameter ControllerMode of CanIf_SetControllerMode() has an invalid value (not CAN_CS_STARTED,
		CAN_CS_SLEEP or CAN_CS_STOPPED), the CanIfshall report development error code CANIF_E_PARAM_CTRLMODE 
		to the Det_ReportError service of the DET module, when CanIf_SetControllerMode() is called.*/
        ErrorStatus            = E_OK               ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CANIF_E_PARAM_CTRLMODE ;
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CTRLMODE);

#endif	 
    }
    else
    {
        /*MISRA*/
    }
    
    
     if ( ErrorStatus == E_NOT_OK )
    {

        /* [SWS_CANIF_00308] The service CanIf_SetControllerMode() shall call
				Can_SetControllerMode(Controller, Transition) for the requested CAN controller.*/
        ErrorStatus = Can_SetControllerMode(ControllerId, ControllerMode);
        if (ErrorStatus == E_NOT_OK )
        {
            CanIf_Global.channelData[channel_CanIf_SetControllerMode].Controller_Mode = ControllerMode;
        }
        else
        {
            /*MISRA*/
        }
        return ErrorStatus ;
    }
  
  
   
  
}


/******************************************* CanIf_getControllerMode ***********************************************/


FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8 ,AUTOMATIC) ControllerId, P2VAR(Can_ControllerStateType,CANIF_CODE,AUTOMATIC) ControllerModePtr)
{
 VAR(Std_ReturnType,AUTOMATIC) ErrorStatus  = E_NOT_OK ;
 VAR(CanIf_Channel_t,AUTOMATIC) channel_CanIf_GetControllerMode = (CanIf_Channel_t) ControllerId;
 
 /*check if INIT function is run */
    
      if (CanIfState == CANIF_UNINIT)
    {
	Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CTRLMODE);    
        return E_NOT_OK;
	   
    }
    else
    {
        /*MISRA*/
    }
    
    
     if(channel_CanIf_GetControllerMode >= MAX_NUM_CHANNELS)
    {
        /*[SWS_CANIF_00313] If parameter ControllerId of CanIf_GetControllerMode() has an invalid, the CanIf shall report
		development error code CANIF_E_PARAM_CONTROLLERID to the Det_ReportError service of the DET, when 
		CanIf_GetControllerMode() is called.*/
        ErrorStatus = E_OK;
        
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CONTROLLERID);
#endif
    }
    else
    {
        /*MISRA*/
    }
    
    
    if(ControllerModePtr == NULL_PTR)
    {
        /* [SWS_CANIF_00656] If parameter ControllerModePtr of CanIf_GetControllerMode() has an invalid value,
		the CanIf shall report development error code CANIF_E_PARAM_POINTER to the Det_ReportError service of
		the DET, when CanIf_GetControllerMode() is called.*/
        ErrorStatus = E_OK;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_MODE_ID,CANIF_E_PARAM_POINTER);
#endif
    }	else
    {
        /*MISRA*/
    }
    if ( ErrorStatus == E_NOT_OK )
    {
        ErrorStatus = Can_GetControllerMode(ControllerId, ControllerModePtr);

    }	else
    {
        /*MISRA*/
    }
    return ErrorStatus ;
    
  }
    
   

/******************************************* CanIf_ControllerBusOff ***********************************************/

FUNC(Std_ReturnType,CANIF_CODE) CanIf_ControllerBusOff(VAR(uint8 ,AUTOMATIC) ControllerId) 
{
VAR(Std_ReturnType,AUTOMATIC) ErrorStatus  = E_NOT_OK ;
VAR(CanIf_Channel_t,AUTOMATIC) channel_CanIf_ControllerBusOff = (CanIf_Channel_t) ControllerId;


/*[SWS_CANIF_00431] If CanIf was not initialized before calling CanIf_ControllerBusOff(),
CanIf shall not execute BusOff notification, when CanIf_ControllerBusOff(),
is called() */

  if (CanIfState == CANIF_UNINIT)
    {
	Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CTRLMODE);    
        return E_NOT_OK;
	   
    }
    else
    {
        /*MISRA*/
    }
  
  
  /*[SWS_CANIF_00429] dIf parameter ControllerId of CanIf_ControllerBusOff() 
  has an invalid value, CanIf shall report development error code CANIF_E_-
  PARAM_CONTROLLERID to the Det_ReportError service of the DET module, when
  CanIf_ControllerBusOff() is called.*/
  
  if(channel_CanIf_ControllerBusOff >= MAX_NUM_CHANNELS)
 {
       ErrorStatus = E_OK;

#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_MODE_ID,CANIF_E_PARAM_POINTER);
#endif
    }	
  
    else
    {
        /*MISRA*/
    }
   
}


/******************************************* CanIf_DeInit ***********************************************/


FUNC(void,CANIF_CODE) CanIf_DeInit(void) {


     CanIfState = CANIF_UNINIT ;

}




/*********************************************************************************************************************************
 Service name:                                              CanIf_Init
 Service ID[hex]:                                               0x01
 Sync/Async:                                               Synchronous
 Reentrancy:                                               Non Reentrant
 Parameters (in):                                           ConfigPtr
 Parameters (inout):                                          None
 Parameters (out):                                             None
 Return value:                                                   None
 Description:
 This service Initializes internal and external interfaces of the CAN Inter-
 face for the further processing.
 *******************************************************************************************************************************/
void CanIf_Init(const CanIf_ConfigType* ConfigPtr)
{

    uint8 Iterator_1 = (uint8) 0;

    if (CanIfState == CANIF_UNINIT)
    {
        /*
         The CanIf expects that the CAN Controller remains in STOPPED mode like after poweron
         reset after the initialization process has been completed. In this mode the CanIf and
         CanDrv are neither able to transmit nor receive CAN L-PDUs (see [SWS_CANIF_00001]).
         */
        for (Iterator_1 = (uint8) 0; Iterator_1 < NUMBER_OF_CONTROLLERS ; Iterator_1++)
        {
            CanIfControllerMode[Iterator_1] = CANIF_CS_STOPPED;
            CanIfPduMode[Iterator_1] = CANIF_OFFLINE;
        }

        CanIfState = CANIF_READY;/*Initialization is done*/
    }
    else
    {
        /* MISRA */
    }
}

/*********************************************************************************************************************************
 Service name:                                         CanIf_SetPduMode
 Service ID[hex]:                                               0x09
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Non Reentrant
 Parameters (in):                                           ControllerId      -->Abstracted CanIf ControllerId which is assigned to a
 CAN controller, which is requested for mode transition.
 PduModeRequest-->Requested PDU mode change
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                              Std_ReturnType
 Description:
 This service sets the requested mode at the L-PDUs of a predefined logical PDU channel.
 *******************************************************************************************************************************/

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId,
                                CanIf_PduModeType PduModeRequest)
{
    CanIf_ControllerModeType CanIfControllerModeLocal;
    if (CanIfState == CANIF_UNINIT)
    {
        #if(CanIfPublicDevErrorDetect == true)
                Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, 0x09, CANIF_E_UNINIT);
        #endif
        return E_NOT_OK;
    }
    else
    {
        /* [SWS_CANIF_00341]: If CanIf_SetPduMode() is called with invalid ControllerId
         , CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
         to the Det_ReportError service of the DET module. c(SRS_BSW_00323)
         */
        if (ControllerId >= NUMBER_OF_CONTROLLERS)
        {
            #if(CanIfPublicDevErrorDetect == true)
                        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, 0x09, CANIF_E_PARAM_CONTROLLERID);
            #endif
            return E_NOT_OK;
        }
        else
        {
            /*
             * [SWS_CANIF_00874] The service CanIf_SetPduMode() shall not accept any request and shall return E_NOT_OK,
             *  if the CCMSM referenced by ControllerId is notin state CANIF_CS_STARTED.
             */

            if (CanIf_GetControllerMode(ControllerId,
                                        &CanIfControllerModeLocal) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {
                if (CanIfControllerModeLocal != CANIF_CS_STARTED)
                {
                    return E_NOT_OK;
                }
                else
                {
                    /*
                     * [SWS_CANIF_00860] If CanIf_SetPduMode() is called with invalid PduModeRequest,
                     CanIf shall report development error code CANIF_E_PARAM_PDU_MODE to the Det_ReportError service of the DET module.
                     */
                    if (PduModeRequest != CANIF_OFFLINE
                            && PduModeRequest != CANIF_TX_OFFLINE
                            && PduModeRequest != CANIF_TX_OFFLINE_ACTIVE
                            && PduModeRequest != CANIF_ONLINE)
                    {
                        #if(CanIfPublicDevErrorDetect == true)
                                                Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, 0x09, CANIF_E_PARAM_PDU_MODE);
                        #endif
                        return E_NOT_OK;

                    }
                    else
                    {
                        CanIfPduMode[ControllerId] = PduModeRequest;
                        return E_OK;
                    }
                }
            }
        }
    }
}




Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr)
{

    /*  CanIf must be initialized after Power ON */
    if (CanIfState == CANIF_UNINIT)
    {
        return E_NOT_OK;
    }
    else
    {
        /* [SWS_CANIF_00346] If CanIf_GetPduMode() is called with invalid ControllerId, CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
         to the Det_ReportError service of the DET module.  */
        if (ControllerId >= NUMBER_OF_CONTROLLERS)
        {
            #if(CanIfPublicDevErrorDetect == true)
                        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, 0x0A, CANIF_E_PARAM_CONTROLLERID);
            #endif
            return E_NOT_OK;
        }
        else
        {
            /*[SWS_CANIF_00657] If CanIf_GetPduMode() is called with invalid PduModePtr,
             CanIf shall report development error code CANIF_E_PARAM_POINTER to the Det_ReportError
             service of the DET module.*/
            if (PduModePtr == NULL)
            {
                #if(CanIfPublicDevErrorDetect == true)
                                Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, 0x0A, CANIF_E_PARAM_POINTER);
                #endif
                return E_NOT_OK;
            }
            else
            {
                *PduModePtr = CanIfPduMode[ControllerId];
                return E_OK;
            }
        }
    }
}

/*********************************************************************************************************************************
 Service name:                                       CanIf_GetTxPdu
 Parameters (in):                        		CanIfTxSduId -->Id of the Tx PDU
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                            CanIfTxPduCfg*
 Description:								This service returns the Tx PDU with the desired ID
 *******************************************************************************************************************************/
CanIfTxPduCfg* CanIf_GetTxPdu(PduIdType CanIfTxSduId)
{
    uint32 TxPduIndex;
    if (CanIfTxSduId < CanIfMaxTxPduCfg)
    {
        for (TxPduIndex = 0; TxPduIndex < CanIfMaxTxPduCfg ; TxPduIndex++)
        {
            // Will be edited when creating the config.c file 
            if (CanIfTxPduCfg[TxPduIndex].CanIfTxPduId
                    == CanIfTxSduId)
            {
                return &CanIfTxPduCfg[TxPduIndex];
            }
            else
            {

            }
        }
    }
    else
    {
        return NULL;
    }

    return NULL;
}

/*********************************************************************************************************************************
 Service name:                                         CanIf_TxConfirmation
 Service ID[hex]:                                               0x13
 Sync/Async:                                                 Synchronous
 Reentrancy:                                                 Reentrant
 Parameters (in):                                           CanTxPduId   -->L-PDU handle of CAN L-PDU successfully transmitted. This ID
                                                                            specifies the corresponding CAN L-PDU ID and implicitly the CAN Driver 
                                                                            instance as well as the corresponding CAN controllerdevice.
 Parameters (inout):                                          None
 Parameters (out):                                            None
 Return value:                                                None
 Description:
 This service confirms a previously successfully processed transmission of a CAN TxPDU.
 *******************************************************************************************************************************/

void CanIf_TxConfirmation (PduIdType CanTxPduId)
{
    CanIfTxPduCfg *TxPduPtr = NULL;
    /*[SWS_CANIF_00412] If CanIf was not initialized before calling CanIf_TxConfir-
    mation(), CanIf shall not call the service <User_TxConfirmation>() and shall
    not set the Tx confirmation status, when CanIf_TxConfirmation() is called.*/
    if (CanIfState == CANIF_UNINIT)
    {
        #if(CanIfPublicDevErrorDetect == true)
            Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, 0x13, CANIF_E_UNINIT);
        #endif
    }else
    {
        if (CanTxPduId > CanIfMaxTxPduCfg)
        {
            /*[SWS_CANIF_00410] If parameter CanTxPduId of CanIf_TxConfirmation()
            has an invalid value, CanIf shall report development error code CANIF_E_PARAM_-
            LPDU to the Det_ReportError service of the DET module, when CanIf_TxCon-
            firmation() is called.*/
            #if(CanIfPublicDevErrorDetect == true)
                Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, 0x13, CANIF_E_PARAM_LPDU);
            #endif

        }else
        {
            TxPduPtr = CanIf_GetTxPdu(CanTxPduId) ;
            if (TxPduPtr == NULL)
            {

            }
            else
            {
                /*[SWS_CANIF_00391] If configuration parameters CanIfPublicReadTxPduNoti-
                fyStatusApi and CanIfTxPduReadNotifyStatus for the Transmitted L-PDU
                are set to TRUE, and if CanIf_TxConfirmation() is called, CanIf shall set the
                notification status for the Transmitted L-PDU.*/
                #if CanIfPublicReadTxPduNotifyStatusApi == true
                    if(TxPduPtr->CanIfTxPduReadNotifyStatus == true)
                    {
                      TxPduState[CanTxPduId]= CANIF_TX_RX_NOTIFICATION;  
                    }else
                    {

                    }
                #endif
                /*[SWS_CANIF_00414] Configuration of CanIf_TxConfirmation(): Each Tx
                L-PDU (see CanIfTxPduCfg) has to be configured with a corresponding transmit
                confirmation service of an upper layer module (see [SWS_CANIF_00011]) which is
                called in CanIf_TxConfirmation().*/
                if (TxPduPtr->CanIfTxPduUserTxConfirmationUL == COM)
                {
                    Com_TxConfirmation(CanTxPduId);
                }
                else if (TxPduPtr->CanIfTxPduUserTxConfirmationUL
                        == PDUR)
                {
                    PduR_CanIfTxConfirmation(CanTxPduId);
                }
                else
                {
                    /* misra */
                }

            }
        }
    }
}


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

    //Check ID if it's Valid unexceed Range it's static id not dynamic
    /* SWS_CANIF_00319 */
    if (TxPduId > CANIF_NUM_TX_PDU_ID) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_INVALID_TXPDUID);
        return E_NOT_OK;
    }

    //Check Controller Mode
    if (CanIf_GetControllerMode(CanIf_ConfigPtr->TxPduCfg.ControllerID, &ControllerMode) != E_OK) {
        return E_NOT_OK;
    }

    //Check PDU Mode
    if (CanIf_GetPduMode(CanIf_ConfigPtr->TxPduCfg.ControllerID, &PduMode) != E_OK) {
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
    CanPdu.length = PduInfoPtr->SduLength;
    CanPdu.id = CanIf_ConfigPtr->TxPduCfg.ID;
    CanPdu.sdu = PduInfoPtr->SduDataPtr;
    CanPdu.swPduHandle = TxPduId;

    /* SWS_CANIF_00162 */
    //RET = Can_Write(CanIf_ConfigPtr->TxPduCfg.HTH, &CanPdu);

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
    if (((MailBox->CanId) < CANID_EXPECTED_MIN) && ((MailBox->CanId) > CANID_EXPECTED_MAX)) {
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
    if (CanIf_GetPduMode(CanIf_ConfigPtr->RxPduCfg.ControllerID, &PduMode) != E_OK) {
        return E_NOT_OK;
    }

    // RX is not online and tx offline active not, report to Det and return
    if (PduMode != CANIF_ONLINE && PduMode != CANIF_TX_OFFLINE_ACTIVE) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_PARAM_PDU_MODE);
        // Rx not online,discard message.
        return E_NOT_OK;
    }

    /* ------------------------------------ Filteraing ------------------------------------ */

    /* Callback Function of *user_RxIndication */

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
    if (CanIf_GetControllerMode(CanIf_ConfigPtr->TxPduCfg.ControllerID, &ControllerMode) != E_OK) {
        return E_NOT_OK;
    }

    // channel not started, report to Det and return
    /* SWS_CANIF_00324 */
    if (ControllerMode != CAN_CS_STARTED) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_CTRLMODE);
        return E_NOT_OK;
    }

    // Copy Data

    return RET;
}

    



