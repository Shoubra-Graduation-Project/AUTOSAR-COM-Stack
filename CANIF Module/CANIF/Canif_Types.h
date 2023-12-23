#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_



#include "CanIf_Cfg.h"
#include "Platform_Types.h"
#include "Commonn_Macros.h"








/****************************   Types Not in SWS    *************************************/


typedef uint8_t Can_ControllerStateType;


typedef enum {
    CANIF_CHANNEL_1,
    CANIF_CHANNEL_2,
    MAX_NUM_CHANNELS
}CanIf_Channel_t;

typedef struct{
    Can_ControllerStateType Controller_Mode;
    CanIf_PduModeType PduMode;
}CanIf_ChannelPrivateType;

typedef struct {
  
    boolean initRun;
    CanIf_ChannelPrivateType channelData[CANIF_CHANNEL_CNT];
    
}CanIf_GlobalType;



#endif /* CANIF_TYPES_H_ */