
#include "CANIF.h"
#include "Std_Types.h"
#include "Commonn_Macros.h"
#include "Canif_Types.h"
#include "CanIf_Cfg.h"
#include "Det.h"











/******************************************* CanIf_SetControllerMode ***********************************************/
LOCAL VAR(CanIf_GlobalType ,AUTOMATIC) CanIf_Global;

FUNC(Std_ReturnType,CANIF_CODE) CanIf_SetControllerMode(VAR(uint8_t,AUTOMATIC) ControllerId,VAR(Can_ControllerStateType , AUTOMATIC) ControllerMode) {
  
    VAR(uint8_t,AUTOMATIC) CanDevolpmentErrorType     = E_OK                          ;
    VAR(Std_ReturnType,AUTOMATIC) ErrorStatus         = E_OK                          ;
    VAR(CanIf_Channel_t ,AUTOMATIC ) channel_CanIf_SetControllerMode = (CanIf_Channel_t) ControllerId;
    
  
  /*check if INIT function is run */
    
     if( CanIf_Global.initRun == (uint8_t)FALSE )
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
  
  
    if ( ControllerMode != CAN_CS_STARTED || ControllerMode != CAN_CS_SLEEP || ControllerMode != CAN_CS_STOPPED)
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
  
    



