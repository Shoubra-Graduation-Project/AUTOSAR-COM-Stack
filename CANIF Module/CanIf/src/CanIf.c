 /* Section : Includes */
#include "../inc/CanIf.h"
#include "../inc/CanIf_Types.h"
#include "../../Common/Compiler.h"
#include "../../Common/Std_Types.h"

 /* Section : Macros Definition */

 /* Section : Macros Functions Declaration */

static CanIfTxPduCfg* CanIf_GetTxPdu(PduIdType CanIfTxSduId);
 
 /* Section : Data Types Declaration */

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
 /* Section : Function Declaration */



STD_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

//STD_ReturnType CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr){}

//void CanIf_RxIndication(Can_HwHandleType hrh, Can_IdType canId, uint8 canDlc, const uint8* canSduPtr, uint8 driverUnit){}


STD_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode ){
    STD_ReturnType RET = E_OK;
    
    return RET;
}

STD_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr ){
    STD_ReturnType RET = E_OK;
    
    return RET;
}



/******************************************* CanIf_SetControllerMode ***********************************************/
LOCAL VAR(CanIf_GlobalType ,AUTOMATIC) CanIf_Global;

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode) {
  
    VAR(uint8,AUTOMATIC) CanDevolpmentErrorType     = E_OK                          ;
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus         = E_OK                          ;
    VAR(CanIf_Channel_t ,AUTOMATIC ) channel_CanIf_SetControllerMode = (CanIf_Channel_t) ControllerId;
    
  
  /*check if INIT function is run */
    
     if( CanIf_Global.initRun == (uint8)FALSE )
    {
        ErrorStatus = E_NOT_OK;
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
      
        ErrorStatus            = E_NOT_OK                   ;
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
        ErrorStatus            = E_NOT_OK               ;
#if CAN_GENERAL_CAN_DEV_ERROR_DETECT == STD_ON
        CanDevolpmentErrorType = CANIF_E_PARAM_CTRLMODE ;
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CTRLMODE);

#endif	 
    }
    else
    {
        /*MISRA*/
    }
    
    
     if ( ErrorStatus == E_OK )
    {

        /* [SWS_CANIF_00308] The service CanIf_SetControllerMode() shall call
				Can_SetControllerMode(Controller, Transition) for the requested CAN controller.*/
        ErrorStatus = Can_SetControllerMode(ControllerId, ControllerMode);
        if (ErrorStatus == E_OK )
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

STD_ReturnType CanIf_SetPduMode(uint8 ControllerId,
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




STD_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr)
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
