#ifndef _COM_STACK_TYPES_H
#define _COM_STACK_TYPES_H
#include "libraries/Std_Types.h"
typedef uint16 PduIdType;
typedef struct 
{
uint32 SduLength;
void *SduDataPtr;
    /* data */
}PduInfoType;


#endif
