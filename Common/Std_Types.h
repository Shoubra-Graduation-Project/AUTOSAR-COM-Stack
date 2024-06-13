 /******************************************************************************
 *
 * Module: Common - Platform
 *
 * File Name: Std_Types.h
 *
 * Description: General type definitions
 *
 * Author:
 *
 *******************************************************************************/

#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

/*****************************************************************************/
/* AUTOSAR Release: 4.3.1                                                      */
/*****************************************************************************/

#include "Platform_Types.h"
#include "Compiler.h"

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

typedef unsigned char			boolean;
typedef unsigned char         	uint8;         
typedef unsigned short        	uint16;        
typedef unsigned long         	uint32;        
typedef signed char           	sint8;       
typedef signed short          	sint16;      
typedef signed long           	sint32;         
typedef signed long long      	sint64;
typedef unsigned long long    	uint64;         
typedef float                 	float32;
typedef double                	float64;

typedef uint8 Std_ReturnType;
typedef unsigned char StatusType;

typedef struct
{
    uint16 vendorID;
    uint16 moduleID;
    uint8  sw_major_version;
    uint8  sw_minor_version;
    uint8  sw_patch_version;
} Std_VersionInfoType;

/*****************************************************************************/
/* SYMBOL DEFINITIONS                                                        */
/*****************************************************************************/
#ifndef STATUSTYPEDEFINED
  #define STATUSTYPEDEFINED
#endif

#define E_OK     0x00u
#define E_NOT_OK   0x01u

#define STD_HIGH   0x01u  /* Physical state 5V or 3.3V */
#define STD_LOW    0x00u  /* Physical state 0v         */

#define STD_ACTIVE 0x01u  /* Logical state active */
#define STD_IDLE   0x00u  /* Logical state idle   */

#define STD_ON     0x01u
#define STD_OFF    0x00u

#define true	1
#define false	0


#endif /*__STD_TYPES_H_ */
