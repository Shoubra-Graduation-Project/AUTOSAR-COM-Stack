#include "CANIF.h"
#include "Std_Types.h"
#include "Commonn_Macros.h"
#include "Canif_Types.h"
#include "CanIf_Cfg.h"
#include "Det.h"







static enum CanIfStateType
{
    CANIF_UNINIT, CANIF_READY
} CanIfState = CANIF_UNINIT;

static CanIf_PduModeType CanIfPduMode[NUMBER_OF_CONTROLLERS];


/******************************************* CanIf_SetControllerMode ***********************************************/
LOCAL VAR(CanIf_GlobalType ,AUTOMATIC) CanIf_Global;

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode) {
  
    VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType     = E_OK                          ;
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus         = E_NOT_OK                          ;
    VAR(CanIf_Channel_t ,AUTOMATIC ) channel_CanIf_SetControllerMode = (CanIf_Channel_t) ControllerId;
    
  
  /*check if INIT function is run */
    
      if (CanIfState == CANIF_UNINIT)
    {
	Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_UNINIT );    
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
  
  
    if ( ControllerMode != CAN_CS_STARTED || ControllerMode != CAN_CS_SLEEP || ControllerMode != CAN_CS_STOPPED)
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


FUNC(Std_ReturnType,CANIF_CODE) CanIf_GetControllerMode(VAR(uint8_t ,AUTOMATIC) ControllerId, P2VAR(Can_ControllerStateType,CANIF_CODE,AUTOMATIC) ControllerModePtr){


 VAR(Std_ReturnType,AUTOMATIC) ErrorStatus  = E_NOT_OK ;
 VAR(CanIf_Channel_t,AUTOMATIC) channel_CanIf_GetControllerMode = (CanIf_Channel_t) ControllerId;
 
 /*check if INIT function is run */
    
      if (CanIfState == CANIF_UNINIT)
    {
	Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_UNINIT);    
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

FUNC(Std_ReturnType,CANIF_CODE) CanIf_ControllerBusOff(VAR(uint8_t ,AUTOMATIC) ControllerId) {

VAR(Std_ReturnType,AUTOMATIC) ErrorStatus  = E_NOT_OK ;
VAR(CanIf_Channel_t,AUTOMATIC) channel_CanIf_ControllerBusOff = (CanIf_Channel_t) ControllerId;


/*[SWS_CANIF_00431] If CanIf was not initialized before calling CanIf_ControllerBusOff(),
CanIf shall not execute BusOff notification, when CanIf_ControllerBusOff(),
is called() */

  if (CanIfState == CANIF_UNINIT)
    {
	Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_SET_CONTROLLER_MODE_ID,CANIF_E_UNINIT);    
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
        Det_ReportError(CANIF_MODULE_ID,CANIF_INSTANCE_ID,CANIF_GET_CONTROLLER_MODE_ID,CANIF_E_PARAM_CONTROLLERID);
#endif
    }	
  
    else
    {
        /*MISRA*/
    }
   
}


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


/******************************************* CanIf_DeInit ***********************************************/


FUNC(void,CANIF_CODE) CanIf_DeInit(void) {


     CanIfState = CANIF_UNINIT ;

}












Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest)
{
    CanIf_ControllerModeType CanIfControllerModeLocal;
    /*
     [SWS_CANIF_00344] d Caveats of CanIf_SetPduMode(): CanIf must be initialized after Power ON.
     */
    if (CanIfState == CANIF_UNINIT)
    {
        return E_NOT_OK;
    }
    else
    {
        /* [SWS_CANIF_00341] d If CanIf_SetPduMode() is called with invalid ControllerId
         , CanIf shall report development error code CANIF_E_PARAM_CONTROLLERID
         to the Det_ReportError service of the DET module. c(SRS_BSW_00323)
         */
        if (ControllerId >= NUMBER_OF_CONTROLLERS)
        {
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


  
    



