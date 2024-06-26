/*************************************************************************************
FileName:                   Can_Cfg.h                                      
AUTOSAR Version:            4.2.2
*************************************************************************************/
#ifndef CAN_CFG_H
#define CAN_CFG_H


/*************************************************************************************
 **                                   Includes                                      **
 *************************************************************************************/

#include "Can_GeneralTypes.h"
#include "../../CanIf Module/inc/CanIf_types.h"
/*************************************************************************************
**                                    Defines                                       **
**************************************************************************************/
/**************************************************************************************************
**
Name:                                     CanDevErrorDetection
Type:                                      EcucBooleanParamDef
Description:            Switches the Default Error Tracer (Det) detection and notification ON or
OFF.
                                        true: enabled (ON).
                                        false: disabled (OFF)                                  **
**************************************************************************************************/
#define CanDevErrorDetection    false

#if !(((CAN_SW_MAJOR_VERSION == 1) && (CAN_SW_MINOR_VERSION == 0)) )
//#error Can: Configuration file expected BSW module version to be 1.0.*
#endif
	


// Number of controller configs
#define CAN_ARC_CTRL_CONFIG_CNT		1

#define CAN_DEV_ERROR_DETECT			STD_ON
#define CAN_VERSION_INFO_API			STD_OFF
#define CAN_MULTIPLEXED_TRANSMISSION	STD_OFF  // Not supported
#define CAN_WAKEUP_SUPPORT				STD_ON  // Not supported
#define CAN_HW_TRANSMIT_CANCELLATION	STD_OFF  // Not supported
#define C
typedef enum
{
	CAN_CTRL_1 = 0,
	CAN_CTRL_2 = 1,
	CAN_CONTROLLER_CNT = 2
}CanControllerIdType;

typedef enum {
	CAN_ID_TYPE_EXTENDED,
	CAN_ID_TYPE_MIXED,
	CAN_ID_TYPE_STANDARD
} Can_IdTypeType;

typedef enum {
	CAN_OBJECT_TYPE_RECEIVE,
	CAN_OBJECT_TYPE_TRANSMIT
} Can_ObjectTypeType;


typedef enum {
	CAN_ARC_HANDLE_TYPE_BASIC,
	CAN_ARC_HANDLE_TYPE_FULL
} Can_Arc_HohType;


typedef enum {
	HWObj_2,
	NUM_OF_HTHS
} Can_Arc_HTHType;


typedef enum {
	HWObj_1,
	NUM_OF_HRHS
} Can_Arc_HRHType;


typedef struct {
	void (*RxIndication)(const Can_HwType*, const PduInfoType*);
	void (*ControllerBusOff)(uint8);
	void (*TxConfirmation)(PduIdType);
	void (*CheckWakeup)(uint8);
//	void (*ControllerModeIndication)(uint8, Can_ControllerStateType);


	//void (*Arc_Error)(uint8,Can_Arc_ErrorType);
} Can_CallbackType;


typedef struct {
	//	Specifies the InstanceId of this module instance. If only one instance is
	//	present it shall have the Id 0
	uint8 CanIndex;
} Can_GeneralType;

// Start mc9s12 unique
typedef enum {
  CAN_ARC_IDAM_2_32BIT,
  CAN_ARC_IDAM_4_16BIT,
  CAN_ARC_IDAM_8_8BIT,
  CAN_ARC_IDAM_FILTER_CLOSED,
} Can_Arc_IDAMType;

typedef uint32 Can_FilterMaskType;

typedef struct Can_HardwareObjectStruct {
	// Specifies the type (Full-CAN or Basic-CAN) of a hardware object.
	Can_Arc_HohType CanHandleType;

	// Specifies whether the IdValue is of type - standard identifier - extended
	// identifier - mixed mode ImplementationType: Can_IdType
	Can_IdTypeType CanIdType;

	//	Specifies (together with the filter mask) the identifiers range that passes
	//	the hardware filter.
	uint32 CanIdValue;

	//	Holds the handle ID of HRH or HTH. The value of this parameter is unique
	//	in a given CAN Driver, and it should start with 0 and continue without any
	//	gaps. The HRH and HTH Ids are defined under two different name-spaces.
	//	Example: HRH0-0, HRH1-1, HTH0-2, HTH1-3
	uint32 CanObjectId;

	// Specifies if the HardwareObject is used as Transmit or as Receive object
	Can_ObjectTypeType CanObjectType;

	// Reference to the filter mask that is used for hardware filtering togerther
	// with the CAN_ID_VALUE
	Can_FilterMaskType *CanFilterMaskRef;

	// A "1" in this mask tells the driver that that HW Message Box should be
	// occupied by this Hoh. A "1" in bit 31(ppc) occupies Mb 0 in HW.
	uint32 Can_Arc_MbMask;	
	
	// End Of List. Set to TRUE is this is the last object in the list.
	boolean Can_Arc_EOL;
} Can_HardwareObjectType;

typedef enum {
	CAN_ARC_PROCESS_TYPE_INTERRUPT,
	CAN_ARC_PROCESS_TYPE_POLLING
} Can_Arc_ProcessType;

typedef struct {

	//	Enables / disables API Can_MainFunction_BusOff() for handling busoff
	//	events in polling mode.
	// INTERRUPT or POLLING
	Can_Arc_ProcessType CanBusOffProcessing;

	// Defines if a CAN controller is used in the configuration.
	boolean CanControllerActivation;

	// Specifies the buadrate of the controller in kbps.
	uint32 CanControllerBaudRate;

	//	This parameter provides the controller ID which is unique in a given CAN
	//	Driver. The value for this parameter starts with 0 and continue without any
	//	gaps.
	uint8 CanControllerId;

	// Specifies propagation delay in time quantas.
	uint32 CanControllerPropSeg;

	// Specifies phase segment 1 in time quantas.
	uint32 CanControllerSeg1;

	// Specifies phase segment 2 in time quantas.
	uint32 CanControllerSeg2;

	//	Specifies the time quanta for the controller. The calculation of the resulting
	//	prescaler value depending on module clocking and time quanta shall be
	//	done offline Hardware specific.
	uint32 CanControllerTimeQuanta;

	//	Enables / disables API Can_MainFunction_Read() for handling PDU
	//	reception events in polling mode.
	Can_Arc_ProcessType CanRxProcessing;

	//	Enables / disables API Can_MainFunction_Write() for handling PDU
	//	transmission events in polling mode.
	Can_Arc_ProcessType CanTxProcessing;

	//	Enables / disables API Can_MainFunction_Wakeup() for handling wakeup
	//	events in polling mode.
	Can_Arc_ProcessType CanWakeupProcessing;

	//	Reference to the CPU clock configuration, which is set in the MCU driver
	//	configuration
	uint32 CanCpuClockRef;

	//	This parameter contains a reference to the Wakeup Source for this
	//	controller as defined in the ECU State Manager. Implementation Type:
	//	reference to EcuM_WakeupSourceType
	uint32 CanWakeupSourceRef;

	// List of Hoh id's that belong to this controller
	const CanHardwareObject  *Can_Arc_Hoh;

	boolean Can_Arc_Loopback;

	// Set this to use the fifo
	boolean Can_Arc_Fifo;

    // the main clock of the uC, used for calculating the baud rate.
	uint32 SystemClock;

} Can_ControllerConfigType;


typedef struct {
	const Can_ControllerConfigType *CanController;
	
	// Callbacks( Extension )
	const Can_CallbackType *CanCallbacks;
	
} Can_ConfigSetType;

/*
typedef struct {
	// This is the multiple configuration set container for CAN Driver
	// Multiplicity 1..*
	const Can_ConfigSetType	 *CanConfigSet;
	// This container contains the parameters related each CAN
	// Driver Unit.
	// Multiplicity 1..*
	const Can_GeneralType	 *CanGeneral;
} Can_ConfigType;*/


extern const Can_ConfigType CanConfigData;
extern const Can_ControllerConfigType CanControllerConfigData[];
extern const Can_ConfigSetType Can_ConfigSet;


uint32 McuE_GetSystemClock();


#endif /*CAN_CFG_H_*/

