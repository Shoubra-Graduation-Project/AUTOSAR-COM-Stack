#ifndef CANIF_TYPES_H_
#define CANIF_TYPES_H_


#include "../../Common/Std_Types.h"
#include "./CanIf_cfg.h"
#include "../../Common/Platform_Types.h"
#include "../../Common/Common_Macros.h"
#include "../../Common/ComStack_Types.h"
#include "../../CanDrv/inc/Can_GeneralTypes.h"
#include "../../CanDrv/inc/Can_Cfg.h"
#include "../../CanDrv/inc/Can.h"






#define CANIF_INSTANCE_ID          1

/****************************   Types Not in SWS    *************************************/
#define NUMBER_OF_CONTROLLERS    (uint8)2
#define NUMBER_OF_CANDRIVERS	 1
#define NUMBER_OF_BUFFERS	 1
#define SDU_LENGTH               8
#define CANID_EXPECTED_MAX       10
#define CANID_EXPECTED_MIN       1
#define CANIF_NUM_RX_PDU_ID      1
#define CANIF_NUM_TX_PDU_ID      1
#define CANIF_CHANNEL_CNT      
#define CANIF_MODULE_ID            1
#define CANIF_INSTANCE_ID          1

/*********************************************************************************************************************************
**                                                                  Development Errors                                                                           **
*********************************************************************************************************************************/

/*Development Errors*/

#define CANIF_E_PARAM_CANID		       (uint8)10  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_HOH	           (uint8)12  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_LPDU  		   (uint8)13  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_CONTROLLER	   (uint8)14  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_CONTROLLERID	   (uint8)15  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_WAKEUPSOURCE	   (uint8)16  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_TRCV             (uint8)17  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_TRCVMODE         (uint8)18  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_TRCVWAKEUPMODE   (uint8)19  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_CTRLMODE         (uint8)21  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_PDU_MODE         (uint8)22  /*API service called with invalid parameter*/

#define CANIF_E_PARAM_POINTER 	       (uint8)20  /*API service called with invalid pointer*/

#define CANIF_E_UNINIT 		           (uint8)30  /*API service used without module initialization*/

#define CANIF_E_INVALID_TXPDUID	       (uint8)50  /*Transmit PDU ID invalid*/

#define CANIF_E_INVALID_RXPDUID        (uint8)60  /*Receive PDU ID invalid */

#define CANIF_E_INVALID_DLC            (uint8)61  /*Failed DLC Check */

#define CANIF_E_DATA_LENGTH_MISMATCH   (uint8)62  /*Data Length */ 

#define CANIF_E_STOPPED                (uint8)70  /*CAN Interface controller mode state machine is in mode CANIF_CS_STOPPED*/

#define CANIF_E_NOT_SLEEP		       (uint8)71  /*CAN Interface controller mode state machine is not in mode CANIF_CS_SLEEP */

#define CANIF_E_INIT_FAILED 	       (uint8)80  /*CAN Interface initialisation failed*/

typedef uint8 Can_ControllerStateType;
typedef uint8 PduIdType;





typedef enum {
    CANIF_CHANNEL_1,
    CANIF_CHANNEL_2,
    MAX_NUM_CHANNELS
}CanIf_Channel_t;

typedef enum {
    CANIF_OFFLINE,
    CANIF_TX_OFFLINE,
    CANIF_TX_OFFLINE_ACTIVE,
    CANIF_ONLINE
}CanIf_PduModeType;

typedef struct{
    Can_ControllerStateType Controller_Mode;
    CanIf_PduModeType PduMode;
}CanIf_ChannelPrivateType;

typedef struct {
  
    boolean initRun;
    CanIf_ChannelPrivateType channelData[CANIF_CHANNEL_CNT];
    
}CanIf_GlobalType; 

/**************************************************************************************************
**
Name:                                   CanIf_NotifStatusType
Type:                                      Enumeration
Description:  
                     Return value of CAN L-PDU notification status.
																								**
**************************************************************************************************/

typedef enum {
    CANIF_TX_RX_NOTIFICATION,
    CANIF_NO_NOTIFICATION
}CanIf_NotifStatusType;

typedef enum {
  /** UNINIT mode. Default mode of the CAN driver and all
   *  CAN controllers connected to one CAN network after
   *  power on. */
  CANIF_CS_UNINIT = 0,

  /**  STOPPED mode. At least one of all CAN controllers
   *   connected to one CAN network are halted and does
   *   not operate on the bus. */
  CANIF_CS_STOPPED,

  /** STARTED mode. All CAN controllers connected to
   *  one CAN network are started by the CAN driver and
   *  in full-operational mode. */
  CANIF_CS_STARTED,

  /** SLEEP mode. At least one of all CAN controllers
   *  connected to one CAN network are set into the
   *  SLEEP mode and can be woken up by request of the
   *  CAN driver or by a network event (must be supported
   *  by CAN hardware) */
  CANIF_CS_SLEEP
} CanIf_ControllerModeType;

typedef struct {

    struct {
    PduIdType nextInQueue; // next tx l-pduid
    }hth[1]; //hth[CANIF_NUM_HTHS];
} CanIf_HthDataType;

typedef struct {
#if CANIF_PUBLIC_SETDYNAMICTXID_API
        Can_IdType dynCanId[CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS];
#endif
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
    } rxLpdu[CANIF_NUM_RX_LPDU_ID];
} CanIf_LPduDataType;

typedef struct {
    /// can id used for transmission, msb indicates extended id
    Can_IdType id;

    /// data length (DLC)
    uint8 dlc;

    /// can driver controller id to be used for transmission
    uint8 controller;

    /// can driver hth id to be used for transmission
    Can_HwHandleType hth;

    /// upper layer confirmation function, set to null if no confirmation
    void(*user_TxConfirmation)(PduIdType txPduId);

    /// upper layer pdu id passed to callout function
    PduIdType ulPduId;
} CanIf_TxPduConfigType;



typedef struct {
    /// can id used for reception filtering
    ///todo add support for range reception
    Can_IdType id;

    /// min dlc and dlc reported to upper layers. Set to -1 to disable dlc check
    uint8 dlc;

    /// can driver controller id from where to receive lpdu
    uint8 controller;

    /** SWS_CANIF_00012
     upper layer indication function, set to null if no rx indication */
    void(*user_RxIndication)(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

    /// upper layer pdu id passed to callout function
    PduIdType ulPduId;
} CanIf_RxLPduConfigType;



#if 0
typedef struct {
  union {
    PduIdType lpduId;
    PduIdType *array;
  }pduInfo;
  PduIdType arrayLen; // 0 means no ptr no filtering = fullCan reception
} CanIf_HrHConfigType;
#endif
#if 0
typedef struct {
} CanIf_ControllerConfigType;

typedef struct {
	void(*user_ControllerModeIndication)(uint8 controllerId, CanIf_ControllerModeType controllerMode);
	void(*user_ControllerBusOff)(uint8 controllerId);
//  void(*user_SetWakeupEvent)(asdf)
//  void(*user_TrcvModeIndication)(asdf)
}CanIf_DispatchCfgType;
#endif



typedef struct
{
	uint8 WakeupSupport;
	uint8 CanIfControllerIdRef;
	uint8 CanIfDriverNameRef[30];
	const CanController* CanIfInitControllerRef;
} CanIf_ControllerConfigType;


typedef struct
{
	void (*CanIfBusOffNotification)(uint8);

	void (*CanIfWakeUpNotification)(uint8);
	void (*CanIfWakeupValidNotification)(uint8);
	void (*CanIfErrorNotificaton)(uint8);
} CanIf_DispatchConfigType;

typedef struct
{
    union {
        PduIdType lpduId;
        PduIdType *array;
    }pduInfo;
    PduIdType arrayLen; // 0 means no ptr no filtering = fullCan reception
}   CanIf_HrHConfigType;



/**************************************************************************************************
**
Name:                                     Can_IdType
Type:                                      uint16, uint32
Description:
                     Represents the Identifier of an L-PDU. The two most significant bits specify the frame type:
                     00 CAN message with Standard CAN ID
                     01 CAN FD frame with Standard CAN ID                                             **
**************************************************************************************************/
typedef uint16  Can_IdType;


typedef struct
{
    /* This parameter references to the logical handle of the underlying CAN
    controller from the CAN Driver module to be served by the CAN
    Interface module. The following parameters of CanController config
    container shall be referenced by this link: CanControllerId,
    CanWakeupSourceRef
    Range: 0..max. number of underlying supported CAN controllers */
    CanController *CanIfCtrlCanCtrlRef;

    /* 
    This parameter abstracts from the CAN Driver specific parameter
    Controller. Each controller of all connected CAN Driver modules shall
    be assigned to one specific ControllerId of the CanIf. Range:
    0..number of configured controllers of all CAN Driver modules 
    */
    uint8 CanIfCtrlId;

    /* 
    This parameter defines if a respective controller of the referenced CAN
    Driver modules is queriable for wake up events.
    True: Enabled False: Disabled 
    */
    boolean CanIfCtrlWakeupSupport;

}CanIfCtrlCfg;

typedef struct {
    /* 
	Reference to controller Id to which the HTH belongs to. A controller
    can contain one or more HTHs. 
	*/
    CanIfCtrlCfg* CanIfHthCanCtrlIdRef;

    /* 
	The parameter refers to a particular HTH object in the CanDrv
    configuration 
	*/
    CanHardwareObject* CanIfHthIdSymRef;

}CanIfHthCfg;

typedef enum {
	/** Transceiver mode NORMAL */
  CANTRCV_TRCVMODE_NORMAL = 0,
  /** Transceiver mode STANDBY */
  CANTRCV_TRCVMODE_STANDBY,
  /** Transceiver mode SLEEP */
  CANTRCV_TRCVMODE_SLEEP
} CanTrcv_TrcvModeType ;
/****************************************************************************/
/*typedef struct {
	/* Everything in this structure is implementation specific 
        const CanIf_TxPduConfigType* TxPduCfg;
	const CanIf_RxLPduConfigType* RxLpduCfg;

    const CanIf_ControllerConfigType* ControllerConfig;
    const CanIf_DispatchConfigType* DispatchConfig;
    const CanIf_HrHConfigType** canIfHrhCfg;  // This is an array of Hrh objects, for each controller ID
} CanIf_ConfigType;*/
/**************************************************************************************************
**
Name:                                	CanIfPduCanIdType
Type:                                 EcucEnumerationParamDef
Description:  			CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN
									L-PDU reception.											**
**************************************************************************************************/
typedef enum{
	/* CAN 2.0 or CAN FD frame with extended identifier (29 bits) */
	EXTENDED_CAN,
	/* CAN FD frame with extended identifier (29 bits) */
	EXTENDED_FD_CAN,
	/* CAN 2.0 frame with extended identifier (29 bits) */
	EXTENDED_NO_FD_CAN,
	/* CAN 2.0 or CAN FD frame with standard identifier (11 bits) */
	STANDARD_CAN,
	/* CAN FD frame with standard identifier (11 bits) */
	STANDARD_FD_CAN,
	/* CAN 2.0 frame with standard identifier (11 bits) */
	STANDARD_NO_FD_CAN
}CanIfPduCanIdType;

/**************************************************************************************************
**
Name:                                  CanIfTxPduType
Type:                                EcucEnumerationParamDef
Description:  			Defines the type of each transmit CAN L-PDU.							**
**************************************************************************************************/
typedef enum{
	/* CAN ID is defined at runtime */
	DYNAMIC,
	/* CAN ID is defined at compile-time. */
	STATIC
}CanIfTxPduType;

/**************************************************************************************************
**
Name:                              CanIfPduUserUL
Type:                                 EcucEnumerationParamDef
Description:  			This parameter defines the upper layer (UL) module to which the
						indication of the successfully received CANRXPDUID has to be routed
						via <User_RxIndication>. This <User_RxIndication> has to be invoked
						when the indication of the configured CANRXPDUID will be received
						by an Rx indication event from the CAN Driver module. If no upper
						layer (UL) module is configured, no <User_RxIndication> has to be
						called in case of an Rx indication event of the CANRXPDUID from the
						CAN Driver module.														**
**************************************************************************************************/
typedef enum{
	CAN_NM,
	CAN_TP,
	CAN_TSYN,
	CDD,
	J1939NM,
	J1939TP,
	PDUR,
	COM,
	XCP
}CanIfPduUserUL;


/**************************************************************************************************
**
Name:                                	  CanIfHrhCfg
Type:                                      Structure
Description:  			This container contains configuration parameters for each hardware
										receive object (HRH).									**
**************************************************************************************************/
typedef struct{
	/* 
    Reference to controller Id to which the HRH belongs to. A controller
	can contain one or more HRHs. 
    */
	CanIfCtrlCfg* CanIfHrhCanCtrlIdRef;
	
	/* 
    The parameter refers to a particular HRH object in the CanDrv
	configuration 
    */
	CanHardwareObject* CanIfHrhIdSymRef;
	
	/* 
    The parameter refers to a particular HRH object in the CanDrv
	configuration 
    */
	boolean CanIfHrhSoftwareFilter;
	
}CanIfHrhCfg;

/**************************************************************************************************
**
Name:                                     CanIfRxPduCanIdRange
Type:                                      Structure
Description:            Optional container that allows to map a range of CAN Ids to one PduId.  **
**************************************************************************************************/
typedef struct{
    /* Lower CAN Identifier of a receive CAN L-PDU for identifier range
    definition, in which all CAN Ids are mapped to one PduId. */
    uint32 CanIfRxPduCanIdRangeLowerCanId;

    /*Upper CAN Identifier of a receive CAN L-PDU for identifier range
    definition, in which all CAN Ids are mapped to one PduId.*/
    uint32 CanIfRxPduCanIdRangeUpperCanId;
}CanIfRxPduCanIdRange;




//extern const CanIf_ConfigType CanIf_Config;
/**************************************************************************************************
**
Name:                                     CanIfRxPduCfg
Type:                                      Structure
Description:  			This container contains the configuration (parameters) of each receive
						CAN L-PDU.
						The SHORT-NAME of "CanIfRxPduConfig" container itself represents
						the symolic name of Receive L-PDU.											**
**************************************************************************************************/
typedef struct{
	/* 
    CAN Identifier of Receive CAN L-PDUs used by the CAN Interface.
	Exa: Software Filtering. This parameter is used if exactly one Can
	Identifier is assigned to the Pdu. If a range is assigned then the
	CanIfRxPduCanIdRange parameter shall be used.
	Range: 11 Bit For Standard CAN Identifier ... 29 Bit For Extended CAN
	identifier 
    */
	uint32 CanIfRxPduCanId;
	
	/* 
    Identifier mask which denotes relevant bits in the CAN Identifier. This
	parameter defines a CAN Identifier range in an alternative way to
	CanIfRxPduCanIdRange. It identifies the bits of the configured CAN
	Identifier that must match the received CAN Identifier. 
    Range: 11 bits
	for Standard CAN Identifier, 29 bits for Extended CAN Identifier. 
    */
	uint32 CanIfRxPduCanIdMask;
	
	/* 
    CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN
	L-PDU reception. 
    */
	CanIfPduCanIdType CanIfRxPduCanIdType;
	
	/* 
    Data length of the received CAN L-PDUs used by the CAN Interface.
	This information is used for DLC checking. Additionally it might specify
	the valid bits in case of the discrete DLC for CAN FD L-PDUs > 8 bytes.
	The data area size of a CAN L-PDU can have a range from 0 to 64
	bytes. 
    */
	uint8 CanIfRxPduDataLength;
	
    /*
    This parameter switches the message specific data length check. True:
    Data length check will be executed during the reception of this PDU.
    False: No data length check will be executed during the reception of
    this PDU.
    */
    boolean CanIfRxPduDataLengthCheck;

    /* 
    ECU wide unique, symbolic handle for receive CAN L-SDU. It shall
	fulfill ANSI/AUTOSAR definitions for constant defines.
	Range: 0..max. number of defined CanRxPduIds 
    */
    uint32 CanIfRxPduId;

    /* 
    Enables and disables the Rx buffering for reading of received L-SDU data.
	True: Enabled False: Disabled 
    */
    boolean CanIfRxPduReadData;
	
	/* 
    Enables and disables receive indication for each receive CAN L-SDU
	for reading its notification status.
	True: Enabled False: Disabled 
    */
     boolean CanIfRxPduReadNotifyStatus;
	
	/* The HRH to which Rx L-PDU belongs to, is referred through this
	parameter. */
     CanIfHrhCfg *CanIfRxPduHrhIdRef;
	
	/* 
    This parameter defines the upper layer (UL) module to which the
	indication of the successfully received CANRXPDUID has to be routed
	via <User_RxIndication>. This <User_RxIndication> has to be invoked
	when the indication of the configured CANRXPDUID will be received
	by an Rx indication event from the CAN Driver module. If no upper
	layer (UL) module is configured, no <User_RxIndication> has to be
	called in case of an Rx indication event of the CANRXPDUID from the
	CAN Driver module. 
    */
	CanIfPduUserUL CanIfRxPduUserRxIndicationUL ;
	
	/* Optional container that allows to map a range of CAN Ids to one PduId. */
	CanIfRxPduCanIdRange CanIfRxPduCanIdRange;

}CanIfRxPduCfg;

/**************************************************************************************************
**
Name:                                	  CanIfInitHohCfg
Type:                                 		Structure
Description:  			This container contains the references to the configuration setup of
										each underlying CAN Driver.											**
**************************************************************************************************/
typedef struct{
	/* This container contains configuration parameters for
	each hardware receive object (HRH) */
	CanIfHrhCfg CanIfHrhCfg[1U];
	
	/* This container contains parameters related to each HTH. */
	CanIfHthCfg CanIfHthCfg[1U];
	
}CanIfInitHohCfg;

/**************************************************************************************************
**
Name:                                	  CanIfCtrlDrvCfg
Type:                                 		Structure
Description:  			Configuration parameters for all the underlying CAN Driver modules
						are aggregated under this container. For each CAN Driver module a
						seperate instance of this container has to be provided.											**
**************************************************************************************************/
typedef struct{
	/* 
    Reference to the Init Hoh Configuration 
    */
	CanIfInitHohCfg* CanIfCtrlDrvInitHohConfigRef;
	

	/* 
    This container contains the configuration (parameters)
	of an adressed CAN controller by an underlying CAN
	Driver module. This container is configurable per CAN
	controller. 
    */
	CanIfCtrlCfg CanIfCtrlCfg[NUMBER_OF_CONTROLLERS];
	
}CanIfCtrlDrvCfg;

/**************************************************************************************************
**
Name:                                	 CanIfBufferCfg
Type:                                      Structure
Description:  			This container contains the Txbuffer configuration. Multiple buffers with
						different sizes could be configured. If CanIfBufferSize
						(ECUC_CanIf_00834) equals 0, the CanIf Tx L-PDU only refers via this
						CanIfBufferCfg the corresponding CanIfHthCfg.											**
**************************************************************************************************/
typedef struct{
	/* 
    This parameter defines the number of CanIf Tx L-PDUs which can be
	buffered in one Txbuffer. If this value equals 0, the CanIf does not
	perform Txbuffering for the CanIf Tx L-PDUs which are assigned to this
	Txbuffer. If CanIfPublicTxBuffering equals False, this parameter equals
	0 for all TxBuffer. If the CanHandleType of the referred HTH equals
	FULL, this parameter equals 0 for this TxBuffer. 
    */
	uint8 CanIfBufferSize;
	
	/* 
    Reference to HTH, that defines the hardware object or the pool of
	hardware objects configured for transmission. All the CanIf Tx L-PDUs
	refer via the CanIfBufferCfg and this parameter to the HTHs if
	TxBuffering is enabled, or not.
	Each HTH shall not be assigned to more than one buffer.
    */
	CanIfHthCfg* CanIfBufferHthRef;
	
	
}CanIfBufferCfg;

/**************************************************************************************************
**
Name:                                     CanIfTxPduCfg
Type:                                      Structure
Description:  			This container contains the configuration (parameters) of a transmit
						CAN L-PDU. It has to be configured as often as a transmit CAN L-PDU is needed.
						The SHORT-NAME of "CanIfTxPduConfig" container represents the
						symolic name of Transmit L-PDU.											**
**************************************************************************************************/
typedef struct{
    /*
    CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for
    CAN L-PDU transmission. Range: 11 Bit For Standard CAN Identifier
    ... 29 Bit For Extended CAN identifier
    The CAN Identifier may be omitted for dynamic transmit L-PDUs.
    */
    uint32 CanIfTxPduCanId;

    /*
    Identifier mask which denotes relevant bits in the CAN Identifier. This
    parameter may be used to keep parts of the CAN Identifier of dynamic
    transmit L-PDUs static. Range: 11 bits for Standard CAN Identifier, 29
    bits for Extended CAN Identifier.
    */
    uint32 CanIfTxPduCanIdMask;

    /*
    Type of CAN Identifier of the transmit CAN L-PDU used by the CAN
    Driver module for CAN L-PDU transmission.
    */
    CanIfPduCanIdType CanIfTxPduCanIdType;

    /*
    ECU wide unique, symbolic handle for transmit CAN L-SDU.
    Range: 0..max. number of CantTxPduIds
    */
    uint32 CanIfTxPduId;
    
    /*
    Enables and disables transmit confirmation for each transmit CAN
	L-SDU for reading its notification status.
	True: Enabled False: Disabled
    */
    boolean CanIfTxPduReadNotifyStatus;

    /*
    Defines the type of each transmit CAN L-PDU.
    */
    CanIfTxPduType	CanIfTxPduType;

    /* 
    Configurable reference to a CanIf buffer configuration. 
    */
    CanIfBufferCfg*     CanIfTxPduBufferRef;

    /* 
    This parameter defines the upper layer (UL) module to which the
	confirmation of the successfully transmitted CANTXPDUID has to be
	routed via the <User_TxConfirmation>. This <User_TxConfirmation>
	has to be invoked when the confirmation of the configured
	CANTXPDUID will be received by a Tx confirmation event from the
	CAN Driver module. If no upper layer (UL) module is configured, no
	<User_TxConfirmation> has to be called in case of a Tx confirmation
	event of the CANTXPDUID from the CAN Driver module. 
    */
     CanIfPduUserUL   CanIfTxPduUserTxConfirmationUL;

}CanIfTxPduCfg;


/**************************************************************************************************
**
Name:                                     CanIfInitCfg
Type:                                      Structure
Description:  			This container contains the init parameters of the CAN Interface.		**
**************************************************************************************************/
typedef struct
{
	/* This container contains the Txbuffer configuration.
	Multiple buffers with different sizes could be configured.
	If CanIfBufferSize (ECUC_CanIf_00834) equals 0, the
	CanIf Tx L-PDU only refers via this CanIfBufferCfg the
	corresponding CanIfHthCfg. */
	CanIfBufferCfg	CanIfBufferCfg[NUMBER_OF_BUFFERS];
	
	/* This container contains the references to the
	configuration setup of each underlying CAN Driver. */
	CanIfInitHohCfg	CanIfInitHohCfg[NUMBER_OF_CANDRIVERS];
	
	/* This container contains the configuration (parameters)
	of each receive CAN L-PDU.
	The SHORT-NAME of "CanIfRxPduConfig" container
	itself represents the symolic name of Receive L-PDU. */
	CanIfRxPduCfg	CanIfRxPduCfg[CanIfMaxRxPduCfg];
	
	/* This container contains the configuration (parameters)
	of a transmit CAN L-PDU. It has to be configured as
	often as a transmit CAN L-PDU is needed.
	The SHORT-NAME of "CanIfTxPduConfig" container
	represents the symolic name of Transmit L-PDU. */
	CanIfTxPduCfg	CanIfTxPduCfg[CanIfMaxTxPduCfg];
	
}CanIfInitCfg;

/**************************************************************************************************
**
Name:                                     CanIf_ConfigType
Type:                                      Structure
Description:  
                     This type defines a data structure for the post build parameters of the CAN
					 interface for all underlying CAN drivers. At initialization the CanIf gets a
					 pointer to a structure of this type to get access to its configuration data, 
					 which is necessary for initialization.
																								**
**************************************************************************************************/
/*typedef struct{
	/* Configuration parameters for all the underlying CAN
	Driver modules are aggregated under this container.
	For each CAN Driver module a seperate instance of
	this container has to be provided. */
	//CanIfCtrlDrvCfg CanIfCtrlDrvCfg;
	
	/* This container contains the init parameters of the CAN
	Interface. 
	CanIfInitCfg CanIfInitCfg;
	
}CanIf_ConfigType;*/

/**************************************************************************************************
**
Name:                                   CanIf_TxBufferType
Type:                                      Structure
Description:   This type defines a data structure for the transmit buffer of the CAN interface	**
**************************************************************************************************/
typedef struct{
	/* Configurable reference to a CanIf buffer configuration. */
	CanIfBufferCfg* CanIfBufferCfgRef;
	/* Pdu data array of the buffer*/
	Can_PduType CanIfTxBufferPduData[CanIfMaxBufferSize];
	/* CanIf Tx queue buffer front index */
	sint8 CanIfTxBufferFront;
	/* CanIf Tx Queue buffer rear index */
	sint8 CanIfTxBufferRear;
	/* CanIf Tx Queue buffer current available PDUs */
	sint8 CanIfTxBufferSize;
	/* States if the PDU is already currently stored in the buffer
	False: Not stored
	True: Stored*/
	boolean CanIfTxBufferPduAvailable[CanIfMaxTxPduCfg];
}CanIf_TxBufferType;

/*************************************************************************************************/
	typedef struct {
	/* Everything in this structure is implementation specific */
    const CanIf_TxPduConfigType* TxPduCfg;
	const CanIf_RxLPduConfigType* RxLpduCfg;

    const CanIf_ControllerConfigType* ControllerConfig;
    const CanIf_DispatchConfigType* DispatchConfig;
    const CanIf_HrHConfigType** canIfHrhCfg;  // This is an array of Hrh objects, for each controller ID
		
		CanIfCtrlDrvCfg CanIfCtrlDrvCfg;
	
		/* This container contains the init parameters of the CAN
		Interface. */
		CanIfInitCfg CanIfInitCfg;
} CanIf_ConfigType;
	
//extern const CanIf_ConfigType CanIf_Config;

#endif /* CANIF_TYPES_H_ */
