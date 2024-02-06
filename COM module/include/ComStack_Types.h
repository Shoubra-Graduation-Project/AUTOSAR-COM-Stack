#ifndef _COM_STACK_TYPES_H
#define _COM_STACK_TYPES_H

/**********************************************************
 * 
 *                     Includes
 * 
 **********************************************************/
#include "libraries/Std_Types.h"

typedef uint16 PduIdType;
typedef uint32 PduLengthType ;

typedef struct 
{
uint32 SduLength;
void *SduDataPtr;
  
}PduInfoType;




typedef enum
{
    BUFREQ_OK,                           //: success
    BUFREQ_E_NOT_OK,                    //: development error detected
    BUFREQ_E_OVFL                      // : requested buffer too large
    
}BufReq_ReturnType ;



#endif
