void Com_Init(const Com_ConfigType *config ) {

    /* Data, deferred data and shadow buffer */
    static uint8 Com_Arc_Buffer[COM_MAX_BUFFER_SIZE];

    DEBUG(DEBUG_LOW, "--Initialization of COM--\n");
    /* !req COM328 */ /* Shall not enable inter-ECU communication */
    /* !req COM483 */

    /* @req COM128 */
    /* @req COM217 */
    /* @req COM444 */
    /* @req COM772 */ /* If timeout set to 0*/

    boolean failure = FALSE;
    /* @req COM433 */
    if( NULL == config ) {
        DET_REPORTERROR(COM_INIT_ID, COM_E_PARAM_POINTER);
        /*lint -e{904} Return statement is necessary in case of reporting a DET error */
        return;
    }
    ComConfig = config;

    boolean dataChanged = FALSE;

    const ComSignal_type *Signal;
    const ComGroupSignal_type *GroupSignal;
    uint16 bufferIndex = 0;

    for (uint16 i = 0; 0 == ComConfig->ComIPdu[i].Com_Arc_EOL; i++) {
        boolean pduHasGroupSignal = FALSE;
        const ComIPdu_type *IPdu = GET_IPdu(i);
        Com_Arc_IPdu_type *Arc_IPdu = GET_ArcIPdu(i);
        Arc_IPdu->Com_Arc_DynSignalLength = 0;
        Arc_IPdu->Com_Arc_IpduRxDMControl = TRUE; /* Enabling by default, assign 0 if not */
        Arc_IPdu->Com_Arc_TxIPduTimers.ComTxDMTimer =  0; /* Initialise Tx deadline timer*/
        if (i >= ComConfig->ComNofIPdus) {
            DET_REPORTERROR(COM_INIT_ID ,COM_E_TOO_MANY_IPDU);
            failure = TRUE;
            break;
        }
        /* Set the data ptr for this Pdu */
        Arc_IPdu->ComIPduDataPtr = (void *)&Com_Arc_Buffer[bufferIndex];
        bufferIndex += IPdu->ComIPduSize;
        /* If this is a TX and cyclic IPdu, configure the first deadline.*/
        if ( (IPdu->ComIPduDirection == COM_SEND) &&
             ((IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeMode == COM_PERIODIC) || (IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeMode == COM_MIXED))) {
            Arc_IPdu->Com_Arc_TxIPduTimers.ComTxModeTimePeriodTimer = IPdu->ComTxIPdu.ComTxModeTrue.ComTxModeTimeOffsetFactor + 1;
        }

        /* Initialize the memory with the default value.*/
        /* @req COM015 */
        if (IPdu->ComIPduDirection == COM_SEND) {
            memset((void *)Arc_IPdu->ComIPduDataPtr, IPdu->ComTxIPdu.ComTxIPduUnusedAreasDefault, IPdu->ComIPduSize);

            /* It is not clear if this value should be set 0 or the configured value. This means that the frames
             * will be sent the first time Com_MainFunctionTx()
             */
            Arc_IPdu->Com_Arc_TxIPduTimers.ComTxIPduMinimumDelayTimer = 0;
        }

        /* For each signal in this PDU.*/

        for (uint16 j = 0; (IPdu->ComIPduSignalRef != NULL) && (IPdu->ComIPduSignalRef[j] != NULL) ; j++) {
            Signal = IPdu->ComIPduSignalRef[j];
            Com_Arc_Signal_type * Arc_Signal = GET_ArcSignal(Signal->ComHandleId);

            /* Configure signal deadline monitoring if used.*/
            /* @req COM333 */ /* If timeout set to 0*/
            /* @req COM292 */ /* handled in generator*/
            /* @req COM290 */ /* handled also in generator*/
            /* the signal does not use an update bit, and should  use per I-PDU deadline monitoring.*/
            /* the signal uses an update bit, and hence has its own deadline monitoring.*/
            if (Signal->ComTimeoutFactor > 0){
                if(IPdu->ComIPduDirection == COM_RECEIVE ){
                    /* Configure the deadline counter */
                    Arc_Signal->Com_Arc_DeadlineCounter = Signal->ComFirstTimeoutFactor;
                } else {
                    /* @req COM445 */ /* handled in generator */
                    /* @req COM481 */
                    /* if one of the signal enabled for DM this means DM exists for this pdu */
                    Arc_IPdu->Com_Arc_TxDeadlineCounter = Signal->ComTimeoutFactor;
                }
            }

            /* Clear update bits*/
            /* @req COM117 */
            if (TRUE == Signal->ComSignalArcUseUpdateBit) {
                /*lint -e{9016} Array indexing couldn't be implemented, as parameters are of different data types */
                CLEARBIT(Arc_IPdu->ComIPduDataPtr, Signal->ComUpdateBitPosition);
            }

            /* If this signal is a signal group*/
            if (TRUE == Signal->Com_Arc_IsSignalGroup) {
                pduHasGroupSignal = TRUE;
                Arc_Signal->Com_Arc_ShadowBuffer = (void *)&Com_Arc_Buffer[bufferIndex];
                /* For each group signal of this signal group.*/
                for(uint8 h = 0; Signal->ComGroupSignal[h] != NULL; h++) {
                    GroupSignal = Signal->ComGroupSignal[h];
                    Com_Arc_GroupSignal_type *Arc_GroupSignal = GET_ArcGroupSignal(GroupSignal->ComHandleId);
                    /* Set pointer to shadow buffer*/
                    /*lint -e{9005} Com_Arc_Buffer can be modified via Arc_GroupSignal->Com_Arc_ShadowBuffer pointer*/
                    Arc_GroupSignal->Com_Arc_ShadowBuffer = ((void *)Arc_Signal->Com_Arc_ShadowBuffer);

                    /* Initialize shadowbuffer.*/
                    /* @req COM484 */
                    Com_Misc_UpdateShadowSignal(GroupSignal->ComHandleId, GroupSignal->ComSignalInitValue);
                }
                /* Initialize group signal data from shadowbuffer.*/
                /* @req COM098 */
                Com_Misc_CopySignalGroupDataFromShadowBufferToPdu(Signal->ComHandleId, false, &dataChanged);
            } else {
                /* Initialize signal data.*/
                /* @req COM098 */
                Com_Misc_WriteSignalDataToPdu(
                        Signal->ComSignalInitValue,
                        Signal->ComSignalType,
                        Arc_IPdu->ComIPduDataPtr,
                        Signal->ComBitPosition,
                        Signal->ComBitSize,
                        Signal->ComSignalEndianess,
                        &dataChanged);
            }
        }
        if( TRUE == pduHasGroupSignal ) {
            /* This pdu has includes group signals. Means that a shodow buffer was set up.
             * Increment index. */
            bufferIndex += IPdu->ComIPduSize;
        }
        if ((IPdu->ComIPduDirection == COM_RECEIVE) && (IPdu->ComIPduSignalProcessing == COM_DEFERRED)) {
            /* Set pointer to the deferred buffer */
            Arc_IPdu->ComIPduDeferredDataPtr = (void *)&Com_Arc_Buffer[bufferIndex];
            bufferIndex += IPdu->ComIPduSize;
            /* Copy the initialized pdu to deferred buffer*/
            memcpy(Arc_IPdu->ComIPduDeferredDataPtr,Arc_IPdu->ComIPduDataPtr,IPdu->ComIPduSize);
        }


    }
    for (uint16 i = 0; i < ComConfig->ComNofIPdus; i++) {
        Com_BufferPduState[i].currentPosition = 0;
        Com_BufferPduState[i].locked = false;
    }

#if(COM_IPDU_COUNTING_ENABLE ==  STD_ON )
    /* @req COM687 */
    ResetInternalCounterRxStartSts();
#endif

    /* An error occurred.*/
    if (TRUE == failure) {
        DEBUG(DEBUG_LOW, "--Initialization of COM failed--\n");

    } else {
        initStatus = COM_INIT;
        DEBUG(DEBUG_LOW, "--Initialization of COM completed--\n");
    }
}
