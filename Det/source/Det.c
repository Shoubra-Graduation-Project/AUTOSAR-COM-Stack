
#include "libraries/Std_Types.h"
#include "Det/include/Det.h"


/***********************************************************************************
 *                                                                                 *
 *    Service Name: Det_Init    
 *    
 *    Service Id: 0x00                                                            *
 * 
 *    Parameters (in):  ConfigPtr ->  Pointer to the selected configuration set
 * 
 *    Parameters (out): None 
 * 
 *    Return Value: None
 * 
 *    Description:  Service to initialize the Default Error Tracer.
 * 
 *********************************************************************************/

void Det_Init (const Det_ConfigType* ConfigPtr)
{

}

void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
	printf("Det Error: ModuleId=%d, InstanceId=%d, ApiId=%d, ErrorId=%d\r\n", \
			ModuleId, InstanceId, ApiId, ErrorId);

}