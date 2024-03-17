#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_


#include "Std_Types.h"
#include "CanIf_Cfg.h"
#include "Platform_Types.h"
#include "Commonn_Macros.h"
#include "ComStack_Types.h"
#include "include/Can_GeneralTypes.h"
#include "include/Can_Cfg.h"






#define CANIF_MODULE_ID            1
#define CANIF_INSTANCE_ID          1

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

typedef enum {
    CANIF_OFFLINE,
    CANIF_TX_OFFLINE,
    CANIF_TX_OFFLINE_ACTIVE,
    CANIF_ONLINE
}CanIf_PduModeType;


typedef enum {
    CANIF_CS_SLEEP,
    CANIF_CS_STARTED,
    CANIF_CS_STOPPED,
    CANIF_CS_UNINIT
}CanIf_ControllerModeType;

typedef struct
{
    /* This parameter references to the logical handle of the underlying CAN
    controller from the CAN Driver module to be served by the CAN
    Interface module. The following parameters of CanController config
    container shall be referenced by this link: CanControllerId,
    CanWakeupSourceRef
    Range: 0..max. number of underlying supported CAN controllers */
    CanController *CanIfCtrlCanCtrlRef;

    /* This parameter abstracts from the CAN Driver specific parameter
    Controller. Each controller of all connected CAN Driver modules shall
    be assigned to one specific ControllerId of the CanIf. Range:
    0..number of configured controllers of all CAN Driver modules */
    uint8 CanIfCtrlId;

    /* This parameter defines if a respective controller of the referenced CAN
    Driver modules is queriable for wake up events.
    True: Enabled False: Disabled */
    bool CanIfCtrlWakeupSupport;

}CanIfCtrlCfg;

typedef struct {
    /* Reference to controller Id to which the HTH belongs to. A controller
    can contain one or more HTHs. */
    CanIfCtrlCfg* CanIfHthCanCtrlIdRef;

    /* The parameter refers to a particular HTH object in the CanDrv
    configuration */
    CanHardwareObject* CanIfHthIdSymRef;

}CanIfHthCfg;

#endif /* CANIF_TYPES_H_ */
