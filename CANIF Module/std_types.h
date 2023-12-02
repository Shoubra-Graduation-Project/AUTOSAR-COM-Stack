/*
 * File:   std_types.h
 * Author: DELL
 *
 * Created on December 1, 2023, 5:07 PM
 */

#ifndef STD_TYPES_H
#define	STD_TYPES_H

 /* Section : Includes */

 /* Section : Macros Functions Declaration */

 /* Section : Data Types Declaration */

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




typedef uint32 STD_ReturnType;


 /* Section : Macros Definition */

#define STD_ACTIVE      0x01
#define STD_IDLE        0x00

#define STD_ON          0x01
#define STD_OFF         0x00

#define STD_HIGH        0x01
#define STD_LOW         0x00

#define E_OK            (STD_ReturnType)0x01
#define E_NOT_OK        (STD_ReturnType)0x00


#define _XTAL_FREQ  16000000UL

 /* Section : Function Declaration */


#endif	/* STD_TYPES_H */

