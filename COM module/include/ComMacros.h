
#ifndef COM_MACROS_H_
#define COM_MACROS_H_


/**********************************************************************************                                                              
 *    Macro Name: GET_SIGNAL    
 *                                                                 
 *    Parameters (in): SignalId
 * 
 *    Return Value: Pointer to the SignalId-th element
 * 
 *    Description:  Get Signal by it's Id from config container
 *********************************************************************************/
#define GET_SIGNAL(SignalId) (&(ComConfig->ComSignal[SignalId]))

/**********************************************************************************                                                                              
 *    Macro Name: GET_IPDU    
 *                                                                 
 *    Parameters (in): IPduId
 * 
 *    Return Value: Pointer to the IPduId-th element
 * 
 *    Description:  Get I-PDU by it's Id from config container
 *********************************************************************************/
#define GET_IPDU(IPduId) (&(ComConfig->ComIPdu[IPduId]))

/**********************************************************************************                                                                            
 *    Macro Name: GET_SIGNALGROUP    
 *                                                                 
 *    Parameters (in): SignalGroupId
 * 
 *    Return Value: Pointer to the SignalGroupId-th element
 * 
 *    Description: Get Signal Group by it's Id from config container
 *********************************************************************************/
#define GET_SIGNALGROUP(SignalGroupId)   ((&(ComConfig->ComSignalGroup[SignalGroupId])))

/***********************************************************************************                                                                               
 *    Macro Name: GET_GROUPSIGNAL    
 *                                                                 
 *    Parameters (in): SignalGroupId, GroupSignalId
 * 
 *    Return Value: Pointer to the GroupSignalId-th element
 * 
 *    Description: Get Group Signal by it's Id from SignalGroup container
 *********************************************************************************/ 
#define GET_GROUPSIGNAL(SignalGroupId,GroupSignalId)  ((&(ComConfig->ComSignalGroup[SignalGroupId]->ComGroupSignal[GroupSignalId])))



/***********************************************************************************                                                                               
 *    Macro Name: CLEARBIT    
 *                                                                 
 *    Parameters (in): dest, bit
 * 
 *    Return Value: 
 * 
 *    Description: clear bit in specific bit
 *********************************************************************************/ 
#define CLEARBIT(dest, bit) \
    (*( (uint8 *)dest + (bit / 8) ) &= (uint8)~(uint8)(1u << (bit % 8)))


#endif