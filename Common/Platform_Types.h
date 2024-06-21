 /******************************************************************************
 *
 * Module: Common - Platform Abstraction
 *
 * File Name: Platform_Types.h
 *
 * Description: Platform types for ARM Cortex-M4F
 *
 * Author:
 *
 *******************************************************************************/

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

/***************************************************************************************************
**                                          Includes                                              **
****************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "Lib/inc/hw_ints.h"
#include "Lib/inc/hw_types.h"
#include "Lib/inc/hw_memmap.h"
#include "Lib/inc/hw_can.h"
#include "Lib/inc/hw_timer.h"

#include "Lib/driverlib/pin_map.h"
#include "Lib/driverlib/can_lab.h"
#include "Lib/driverlib/interrupt.h"
#include "Lib/driverlib/sysctl.h"
#include "Lib/driverlib/gpio.h"
#include "Lib/driverlib/debug.h"
#include "Lib/utils/uartstdio.h"
#include "Lib/driverlib/uart.h"
#include "Lib/driverlib/timer.h"

/*****************************************************************************/
/* AUTOSAR Release: 4.3.1                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* GLOBAL DEFINITIONS                                                        */
/*****************************************************************************/
#define CPU_TYPE_8          8
#define CPU_TYPE_16         16
#define CPU_TYPE_32         32

#define MSB_FIRST           0
#define LSB_FIRST           1

#define HIGH_BYTE_FIRST     0
#define LOW_BYTE_FIRST      1

#define TRUE              1

#define FALSE             0

/*****************************************************************************/
/* REGISTER WIDTH                                                            */
/*****************************************************************************/
#define CPU_TYPE            CPU_TYPE_32

/*****************************************************************************/
/* ENDIANESS                                                                 */
/*****************************************************************************/
#define CPU_BIT_ORDER       LSB_FIRST

#if __little_endian__
  #define CPU_BYTE_ORDER    LOW_BYTE_FIRST
#else
  #define CPU_BYTE_ORDER    HIGH_BYTE_FIRST
#endif

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
/*typedef unsigned char       boolean;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long	uint64;

typedef signed char         sint8;
typedef signed short        sint16;
typedef signed int          sint32;
typedef signed long long		sint64;

typedef unsigned int        uint8_least;
typedef unsigned int        uint16_least;
typedef unsigned int        uint32_least;

typedef signed int          sint8_least;
typedef signed int          sint16_least;
typedef signed int          sint32_least;

typedef float               float32;
typedef double              float64;*/

#endif /* _PLATFORM_TYPES_H_ */
