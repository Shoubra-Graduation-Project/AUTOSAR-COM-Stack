#ifndef _DET_H_
#define _DET_H_


void Det_Init (const Det_ConfigType* ConfigPtr);
void Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);


#endif