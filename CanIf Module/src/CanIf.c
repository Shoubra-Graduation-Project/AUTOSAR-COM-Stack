#include "../inc/CanIf.h"
#include "../inc/CanIf_types.h"
#include "../inc/CanIf_Cfg.h"
#include "../../Common/ComStack_Types.h"
#include "../../Common/Std_Types.h"
#include "../../Common/Common_Macros.h"
#include "../../CanDrv/Inc/Can_GeneralTypes.h"
#include "../../CanDrv/Inc/Can.h"
#include "../../Det/inc/Det.h"
#include "../../Common/Lib/driverlib/pin_map.h"
#include "../../Common/Lib/driverlib/gpio.h"
#include "../../Common/Lib/driverlib/sysctl.h"
#include "../../Common/Lib/driverlib/uart.h"
#include "../../Common/Integrator.h"

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode);
Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr);
FUNC(void,CANIF_CODE) CanIf_DeInit(void);
Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest);
Std_ReturnType CanIf_GetPduMode(uint8 ControllerId, CanIf_PduModeType* PduModePtr);
void CanIf_Init(const CanIf_ConfigType* ConfigPtr);
void CanIf_TxConfirmation (PduIdType CanTxPduId);
Std_ReturnType CanIf_SetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode );
Std_ReturnType CanIf_GetTrcvMode( uint8 TransceiverId, CanTrcv_TrcvModeType* TransceiverModePtr );
Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
Std_ReturnType CanIf_RxIndication(const Can_HwType* MailBox, const PduInfoType* PduInfoPtr);
Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr);

extern CanIf_ConfigType CanIf;

static CanIf_ControllerModeType CanIfControllerMode[NUMBER_OF_CONTROLLERS] = {
        CANIF_CS_UNINIT, CANIF_CS_UNINIT };

static CanIf_PduModeType CanIfPduMode[NUMBER_OF_CONTROLLERS];

static inline int LockSave(void)
{
    return 0;
}

static inline void LockRestore(int msr)
{
}

struct {
#if CANIF_PUBLIC_TX_BUFFERING
        PduIdType nextInQueue;
        uint8 data[8];
    // dlc set to -1 indicates empty buffer
        uint8 dlc;
#endif
#if CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
        bool txConfirmed;
#endif
    } txLpdu[CANIF_NUM_TX_PDU_ID];


struct {
#if CANIF_PUBLIC_READRXPDU_DATA_API
        uint8 data[8];
        uint8 dlc;
#endif
#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API
        bool rxInd;
#endif
}rxLpdu[CANIF_NUM_RX_LPDU_ID];



#if(CanIfPublicReadRxPduDataApi == true)
static PduInfoType RxBuffer[CanIfMaxRxPduCfg];
#endif

#if(CanIfPublicReadTxPduNotifyStatusApi==true)
CanIf_NotifStatusType TxPduState[CanIfMaxTxPduCfg];
#endif

#if(CanIfPublicReadRxPduNotifyStatusApi == true)
static CanIf_NotifStatusType RxPduState[CanIfMaxRxPduCfg];
#endif

#if(CanIfPublicTxBuffering == false)
static CanIf_TxBufferType CanIfTxBuffer[NUMBER_OF_BUFFERS] = { {
        .CanIfBufferCfgRef = &CanIf.CanIfInitCfg.CanIfBufferCfg[0U],
        .CanIfTxBufferFront = -1, .CanIfTxBufferRear = -1, .CanIfTxBufferSize =
                -1,
        .CanIfTxBufferPduAvailable = { false } } };
#endif

static enum CanIfStateType
{
    CANIF_UNINIT, CANIF_READY
} CanIfState = CANIF_UNINIT;

static CanIf_PduModeType CanIfPduMode[NUMBER_OF_CONTROLLERS];

static const CanIf_ConfigType* CanIf_ConfigPtr;
PduInfoType* CanifBuffer;

static void ClearTxBuffers(uint8 controller) {
    // reset all pending tx requests
		CanIf_LPduDataType lPduData;
		CanIf_HthDataType hthData;
    for(PduIdType i = 0; i < CANIF_NUM_TX_PDU_ID; i++) {
        if(CanIf_ConfigPtr->TxPduCfg[i].controller == controller) {
#if CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
      // clear notification status
      lPduData.txLpdu[i].txConfirmed = CANIF_NO_NOTIFICATION;
#endif
#if CANIF_PUBLIC_TX_BUFFERING
            // set nextInQueue to indicate empty queue
            hthData.hth[CanIf_ConfigPtr->TxPduCfg[i].hth].nextInQueue = -1;
          // set dlc to -1 to indicate empty buffer
            lPduData.txLpdu[i].dlc = -1;
#endif
        }
    }
}


static void ClearRxBuffers(uint8 controller) {
    // reset buffers
		CanIf_HthDataType hthData;
	 CanIf_LPduDataType lPduData;
    for(PduIdType i = 0; i < CANIF_NUM_RX_LPDU_ID; i++) {
        if(CanIf_ConfigPtr->RxLpduCfg[i].controller == controller) {
#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API
      // clear notification status
      lPduData.rxLpdu[i].rxInd = CANIF_NO_NOTIFICATION;
#endif
#if CANIF_PUBLIC_READRXPDU_DATA_API
          // set dlc to -1 to indicate empty buffer
            lPduData.rxLpdu[i].dlc = -1;
#endif
        }
    }
}
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
    //CanIf_SetPduMode();
    //Can_SetControllerMode();
     if (CAN_CS_SLEEP == ControllerMode)
    {
        CanIf_SetPduMode(ControllerId, CANIF_OFFLINE);
    }
    else if (CAN_CS_STOPPED == ControllerMode)
    {
        CanIf_SetPduMode(ControllerId, CANIF_TX_OFFLINE);
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
  
  
    return Can_SetControllerMode(ControllerId, ControllerMode);

  
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
        //ErrorStatus = Can_GetControllerMode(ControllerId, ControllerModePtr);

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
// reset all buffers
    int lock = LockSave();
    ClearTxBuffers(ControllerId);
    ClearRxBuffers(ControllerId);
    LockRestore(lock);	
	
   
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
        #if(CanIfPublicReadTxPduNotifyStatusApi == true)
        for(Iterator_1 = 0; Iterator_1 < CanIfMaxTxPduCfg; Iterator_1++)
        {
            TxPduState[Iterator_1] = CANIF_NO_NOTIFICATION;
        }
        #endif
        #if(CanIfPublicReadRxPduNotifyStatusApi == true)
                for(Iterator_1 = 0; Iterator_1 < CanIfMaxRxPduCfg; Iterator_1++)
                {
                    RxPduState[Iterator_1] = CANIF_NO_NOTIFICATION;
                }
        #endif

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
    /*
     [SWS_CANIF_00344] d Caveats of CanIf_SetPduMode(): CanIf must be initialized after Power ON.
     */
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
            Can_MainFunction_Mode();
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
            if (CanIf.CanIfInitCfg.CanIfTxPduCfg[TxPduIndex].CanIfTxPduId
                                        == CanIfTxSduId)
            {
                return &CanIf.CanIfInitCfg.CanIfTxPduCfg[TxPduIndex];
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
    CanIf_ControllerModeType CanIfControllerModeLocal;
    Can_PduType PduInfoPtr;
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
                /*[SWS_CANIF_00740] d If CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT (see ECUC_CanIf_00
                 is enabled, CanIf shall buffer the information about a received TxConfirmation per
                 CAN Controller, if the CCMSM of that controller is in state CANIF_CS_STARTED.*/
                #if(CanIfPublicTxConfirmPollingSupport == true)
                                if(CanIf_GetControllerMode(txpduptr_0x13->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId, &CanIfControllerModeLocal) == E_NOT_OK)
                                {

                                }
                                else
                                {
                                    if(CanIfControllerModeLocal != CANIF_CS_STARTED)
                                    {

                                    }
                                    else
                                    {
                                        TxPduState[CanTxPduId]= CANIF_TX_RX_NOTIFICATION;
                                    }
                                }
                #endif
                #if(CanIfPublicTxBuffering == true)
                                Can_ReturnType transmitcheck_0x13;
                                if (CanIf_TxBufferPeek(txpduptr_0x13, &PduInfoPtr) == E_NOT_OK)
                                {

                                }
                                else
                                {
                                    /* Attempt to transmit a PDU from the buffer */
                                    transmitcheck_0x13 =
                                            Can_Write(
                                                    txpduptr_0x13->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthIdSymRef->CanObjectId,
                                                    &PduInfoPtr);
                                    if (transmitcheck_0x13 == CAN_OK)
                                    {
                                        CanIf_TxBufferDequeue(txpduptr_0x13, &PduInfoPtr);
                                    }
                                    else
                                    {

                                    }
                                }
                #endif
                #if(CanIfPublicReadTxPduNotifyStatusApi == true)
                                /*
                                * [SWS_CANIF_00391] d If configuration parameters CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS
                                (ECUC_CanIf_00609) and CANIF_TXPDU_READ_NOTIFYSTATUS (ECUC_CanIf_00589)
                                for the Transmitted L-PDU are set to TRUE, and if CanIf_TxConfirmation() is
                                called, CanIf shall set the notification status for the Transmitted L-PDU.
                                */
                                if(txpduptr_0x13->CanIfTxPduReadNotifyStatus == true)
                                {
                                    TxPduState[CanTxPduId]= CANIF_TX_RX_NOTIFICATION;
                                }
                                else
                                {

                                }
                #endif
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
                    Com_TxConfirmation(CanTxPduId, E_OK);
                }
                else if (TxPduPtr->CanIfTxPduUserTxConfirmationUL
                        == PDUR)
                {
                    Com_TxConfirmation(CanTxPduId, E_OK);
                }
                else
                {
                    /* misra */
                }

            }
        }
    }
}


/* Find ID of Transmit Pdu */
const CanIfTxPduCfg* CanIf_FindTxPduEntry(PduIdType TxPduId) 
{
    if (TxPduId >= CanIfMaxTxPduCfg) {
        return (CanIfTxPduCfg*)NULL;
    }

    uint32 Index, i;
    for (i = 0; i < CanIfMaxTxPduCfg; i++) {
        if (TxPduId == (CanIf_ConfigPtr->CanIfInitCfg.CanIfTxPduCfg[i].CanIfTxPduId)) {
            Index = i;
            break;
        }
    }
    return &CanIf_ConfigPtr->CanIfInitCfg.CanIfTxPduCfg[Index];
}

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) 
{

    Std_ReturnType RET = E_OK;
    Can_PduType CanPdu;
    const CanIfTxPduCfg* TxEntry;
    CanIf_ControllerModeType ControllerMode = (CanIf_ControllerModeType)0;
    CanIf_PduModeType PduMode = (CanIf_PduModeType)0;

    //Check CAN is INITIATE or Not
    if (CanIfState != CANIF_READY) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_ID, CANIF_E_UNINIT);
        return E_NOT_OK;
    }

    //Check pointer != Null
    /* SWS_CANIF_00320 */
    if (PduInfoPtr == NULL) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKVALIDATION_ID, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    //Check ID if it's Valid
    /* SWS_CANIF_00319 */
    TxEntry = CanIf_FindTxPduEntry(TxPduId);
    if (TxEntry == 0) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_INVALID_TXPDUID);
        return E_NOT_OK;
    }

    uint8 Controller_ID = (uint8)TxEntry->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthCanCtrlIdRef->CanIfCtrlId;

    //Check Controller Mode
    if (CanIf_GetControllerMode(Controller_ID, &ControllerMode) != E_OK) {
        return E_NOT_OK;
    }

    //Check PDU Mode
    if (CanIf_GetPduMode(Controller_ID, &PduMode) != E_OK) {
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

    /* Copy data to CanPdu */
    CanPdu.length = PduInfoPtr->SduLength;
    CanPdu.id = TxEntry->CanIfTxPduCanId;;
    CanPdu.sdu = PduInfoPtr->SduDataPtr;
    CanPdu.swPduHandle = TxPduId;

    /* SWS_CANIF_00162 */
    RET = Can_Write(TxEntry->CanIfTxPduBufferRef->CanIfBufferHthRef->CanIfHthIdSymRef->CanObjectId, &CanPdu);

    return RET;
}


/* To Find RxPdu*/
const CanIfRxPduCfg* CanIf_FindRxPduEntry(Can_HwHandleType Hoh) {
    if (Hoh >= CanIfMaxRxPduCfg) {
        return (CanIfRxPduCfg*)NULL;
    }
    uint32 Index = 0, i;
    for (i = 0; i < CanIfMaxRxPduCfg; i++) {
        printf("Entry of [%d] = [%d]\n", i, CanIf_ConfigPtr->CanIfInitCfg.CanIfRxPduCfg[i].CanIfRxPduHrhIdRef->CanIfHrhIdSymRef->CanObjectId);
        if (Hoh == CanIf_ConfigPtr->CanIfInitCfg.CanIfRxPduCfg[i].CanIfRxPduHrhIdRef->CanIfHrhIdSymRef->CanObjectId) {
            Index = i;
            break;
        }
    }
    if (i == CanIfMaxRxPduCfg) {
        return (CanIfRxPduCfg*)NULL;
    }
    return &CanIf_ConfigPtr->CanIfInitCfg.CanIfRxPduCfg[Index];
}


Std_ReturnType CanIf_RxIndication(const Can_HwType* MailBox, const PduInfoType* PduInfoPtr) 
{
    Std_ReturnType RET = E_OK;
    CanIf_PduModeType PduMode = (CanIf_PduModeType)0;
    const CanIfRxPduCfg* RxPduIndex = CanIf_FindRxPduEntry(MailBox->Hoh);
    uint8 ControllerID = (uint8)RxPduIndex->CanIfRxPduHrhIdRef->CanIfHrhCanCtrlIdRef->CanIfCtrlId;
    const CanIfRxPduCfg* RxEntry;

 
    //Check CAN is INITIATE or Not
    if (CanIfState != CANIF_READY) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_INIT_ID, CANIF_E_UNINIT);
        return E_NOT_OK;
    }

    //Check CAN is INITIATE or Not & Check pointer != Null & MailBox
    /* SWS_CANIF_00419 */
    if ((MailBox == NULL) || (PduInfoPtr == NULL) || ((PduInfoPtr->SduDataPtr) == NULL)) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_CHECKVALIDATION_ID, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    // Check if MailBox->HOH has Invalid Value
    /* SWS_CANIF_00416 */
    if ((MailBox->Hoh) > NUMBER_OF_HOH) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_ID, CANIF_E_PARAM_POINTER);
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
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_ID, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    CanIf_Channel_t Controller_ID = (CanIf_Channel_t)RxEntry->CanIfRxPduHrhIdRef->CanIfHrhCanCtrlIdRef->CanIfCtrlId;

    //Check PDU Mode
    if (CanIf_GetPduMode(Controller_ID, &PduMode) != E_OK) {
        return E_NOT_OK;
    }

    // RX is not online and tx offline active not, report to Det and return
    if (PduMode != CANIF_ONLINE && PduMode != CANIF_TX_OFFLINE_ACTIVE && PduMode != CANIF_TX_OFFLINE) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_PARAM_PDU_MODE);
        // Rx not online,discard message.
        return E_NOT_OK;
    }

    /* ------------------------------------ Filteraing & Copy Data To Recieving Buffer Of CanIf ------------------------------------ */

    // Check IDs (MailBox)
    /* SWS_CANIF_00416 */
    if (RxPduIndex == NULL) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_PDU_MODE_ID, CANIF_E_INVALID_TXPDUID);
        return E_NOT_OK;
    }

    // Check if Data Length has invalid Value
    /* SWS_CANIF_00417 */
    if ((PduInfoPtr->SduLength) != (RxPduIndex->CanIfRxPduDataLength)) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_RX_INDICATION_ID, CANIF_E_INVALID_TXPDUID);
        return E_NOT_OK;
    }

    // Copy data from CAN to CanIf buffer
    CanifBuffer->SduDataPtr = PduInfoPtr->SduDataPtr;
    CanifBuffer->SduLength = PduInfoPtr->SduLength;

    /* Confirmation to Upper Layer (COM)*/
    //(RxPduIndex->CanIfRxPduUserRxIndicationUL);
		Com_RxIndication (RxPduIndex->CanIfRxPduId, PduInfoPtr);
	
    return RET;
}


Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr)
{
    Std_ReturnType RET = E_OK;
    const CanIfRxPduCfg* RxEntry;
    CanIf_ControllerModeType ControllerMode = (CanIf_ControllerModeType)0;

    //Check CAN is INITIATE or Not
    if (CanIfState != CANIF_READY) {
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
    if (CanIfRxSduId >= CanIfMaxRxPduCfg) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_READRXNOTIFSTATUS_ID, CANIF_E_INVALID_RXPDUID);
        return E_NOT_OK;
    }

    uint8 Controller_ID = (uint8)RxEntry->CanIfRxPduHrhIdRef->CanIfHrhCanCtrlIdRef->CanIfCtrlId;


    //Check Controller Mode
    if (CanIf_GetControllerMode(Controller_ID, &ControllerMode) != E_OK) {
        return E_NOT_OK;
    }

    // channel not started, report to Det and return
    /* SWS_CANIF_00324 */
    if (ControllerMode != CAN_CS_STARTED) {
        Det_ReportError(CANIF_MODULE_ID, CANIF_INSTANCE_ID, CANIF_SET_CONTROLLER_MODE_ID, CANIF_E_PARAM_CTRLMODE);
        return E_NOT_OK;
    }

    // Copy Data from CanIf Buffer
    CanIfRxInfoPtr->SduLength = CanifBuffer->SduLength;
    CanIfRxInfoPtr->SduDataPtr = CanifBuffer->SduDataPtr;

    return RET;
}
    
