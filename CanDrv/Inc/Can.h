/********************************************************************************************************************************
**
FileName:                   Can.h
AUTOSAR Version:            4.2.2
																											                   **
*********************************************************************************************************************************/
#ifndef CAN_H
#define CAN_H
/********************************************************************************************************************************
**                                                 Includes                                                                    **
*********************************************************************************************************************************/
#include "Can_GeneralTypes.h"
#include "../../Common/ComStack_Types.h"
#include "../../CanIf Module/inc/CanIf.h"
#include "Can_Cfg.h"

extern Can_ConfigType Can;
/*******************************************************************************************************************************
**                                                 Defines                                                                    **
********************************************************************************************************************************/
#define CONTROLLER_ZERO            0U
#define CONTROLLER_ONE             1U
#define MIN_OBJECT_HANDLER         0x01U
#define MAX_OBJECT_HANDLER         0x20U
#define INDEX_ZERO                 0U
#define INDEX_ONE                  1U

//These Are the Values of The Control Registers After a Reset Brought From TM4CGH6PM Data Sheet
#define RST_CANCTL                0x00000001
#define RST_CANTST                0x00000000
#define RST_CANBRPE               0x00000000
#define RST_CANBIT                0x00002301
#define RST_CANERR                0x00000000

#define CAN0_IF1CRQ_R           (*((volatile uint32_t *)0x40040020))
#define CAN0_IF1MCTL_R          (*((volatile uint32_t *)0x40040038))
#define 	CANCTL                0x000
#define 	CANIF1MCTL            0x038
#define 	CANIF1CMSK            0x024
#define 	CANIF1CRQ             0x020
#define WRNRD_Bit               7
#define NEWDAT_Bit              15
#define CANIF1CMSK_TXRQST_Bit   2
#define   GET_ADDRESS_VAL(A,B)           *((volatile uint32_t *)((A)+(B)))
#define    CLEAR_BIT(A,B,P)                    *((volatile uint32_t *)((A)+(B))) & (~(1 << P))
#define    SET_BIT(A,B,P)                          *((volatile uint32_t *)((A)+(B)))|(1 << P))
#define     status_Initialization   (GET_ADDRESS_VAL(CanControllerBaseAddress,CANCTL))&CAN_CTL_INIT
#define      GetBit(reg,n)              ((reg>>n)&1)

/*
typedef struct Can_ControllerConfig {

     bool CanControllerActivation;

     // Specifies the buadrate of the controller in kbps.
     uint32 CanControllerBaudRate;

     //  This parameter provides the controller ID which is unique in a given CAN
     //  Driver. The value for this parameter starts with 0 and continue without any
     //  gaps.

     CanControllerIdType CanControllerId;

     // Specifies propagation delay in time quantas.
     uint8 CanControllerPropSeg;

     // Specifies phase segment 1 in time quantas.
     uint8 CanControllerSeg1;

     // Specifies phase segment 2 in time quantas.
     uint8 CanControllerSeg2;

     // Specifies Reset Jump Width register value.
     uint8 CanControllerRJW;

     //  Specifies the time quanta for the controller. The calculation of the resulting
     //  prescaler value depending on module clocking and time quanta shall be
     //  done offline Hardware specific.
//     uint32 CanControllerTimeQuanta;

     //  Reference to the CPU clock configuration, which is set in the MCU driver
     //  configuration
     uint32 CanCpuClockRef;

     //  This parameter contains a reference to the Wakeup Source for this
     //  controller as defined in the ECU State Manager. Implementation Type:
     //  reference to EcuM_WakeupSourceType
     uint32 CanWakeupSourceRef;

     //Flags, See CAN_CTRL_XX macros 
     uint32 Can_Arc_Flags;

     /* Number of FIFO MB in the HOH list 
     uint8 Can_Arc_HohFifoCnt;

     /* Total number of HOHs in Can_Arc_Hoh 
     uint8 Can_Arc_HohCnt;

     // List of Hoh id's that belong to this controller
     const Can_HardwareObjectType  * const Can_Arc_Hoh;

     uint64 Can_Arc_RxMailBoxMask;
     uint64 Can_Arc_TxMailBoxMask;

     const uint8 * const Can_Arc_MailBoxToHrh;

     PduIdType * const Can_Arc_TxPduHandles;

     uint8 Can_Arc_TxMailboxStart;

     uint8 Can_Arc_MailboxMax;

#if 1
    //uint32 flags;
#else
     boolean Can_Arc_Loopback;

     // Set this to use the fifo
     boolean Can_Arc_Fifo;
#endif

 } Can_ControllerConfigType;*/



/********************************************************************************************************************************/

/********************************************************************************************************************************
**                                                  Function Prototypes                                                       **
*********************************************************************************************************************************/
void Can_MainFunction_Mode(void);
void Can_MainFunction_Busoff(void);
Can_ReturnType    Can_SetControllerMode (uint8 Controller,Can_StateTransitionType Transition);
void Can0_InterruptHandler(void);
Can_ReturnType    Can_Write( Can_HwHandleType Hth,const Can_PduType* PduInfo);
void Can_EnableControllerInterrupts(uint8 Controller);
void Can1_InterruptHandler(void);
void Can_DisableControllerInterrupts(uint8 Controller);
void Can_Init(const Can_ConfigType* Config);
Std_ReturnType Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID );
void Can_MainFunctionBusoff(void);
void Can_MainFunction_Write(void);
void Can_MainFunction_Read(void);
void Can_ISRHandler(uint8 Controller);
/********************************************************************************************************************************/
#endif /* CAN_H */

