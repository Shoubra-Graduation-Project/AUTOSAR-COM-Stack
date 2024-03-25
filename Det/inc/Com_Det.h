
#ifndef _COM_DET_H_
#define _COM_DET_H_

// [SWS_Com_00803] API service called with wrong parameter
#define COM_E_PARAM                  0x01

/* [SWS_Com_00804] Error code if any API service (except Com_GetStatus,
Com_GetVersionInfo, Com_MainFunctionRx, Com_MainFunctionTx and
Com_MainFunctionRouteSignals) is called before the AUTOSAR COM module was
initialized with Com_Init or after a call to Com_DeInit*/
#define COM_E_UNINIT                 0x02

// [SWS_Com_00805] NULL pointer checking
#define COM_E_PARAM_POINTER          0x03

// [SWS_Com_00837] Invalid configuration set selection
#define COM_E_INIT_FAILED            0x04

// [SWS_Com_00864] Transmission request was skipped
#define COM_E_SKIPPED_TRANSMISSION   0x05





#define COM_MODULE_ID
#define COM_INSTANCE_ID 0

#define COM_INIT_ID                                0x01
#define COM_DEINIT_ID                              0x02
#define COM_SEND_SIGNAL_ID                         0x0a
#define COM_RECEIVE_SIGNAL_ID                      0x0b
#define COM_SEND_SIGNAL_GROUP_ID                   0x0d
#define COM_RECEIVE_SIGNAL_GROUP_ID                0x0e

#define COM_RX_INDICATION_ID                       0x42
#define COM_TP_RX_INDICATION_ID                    0x45
#define COM_TX_CONFIRMATION_ID                     0x40
#define COM_TP_TX_CONFIRMATION_ID                  0x48
#define COM_START_OF_RECEPTION_ID                  0x46

#define COM_MAIN_FUNCTION_RX_ID                    0x18
#define COM_MAIN_FUNCTION_TX_ID                    0x19

#endif
