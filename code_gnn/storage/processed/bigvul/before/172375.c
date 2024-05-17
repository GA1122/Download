OMX_ERRORTYPE  omx_video::send_command_proxy(OMX_IN OMX_HANDLETYPE hComp,
        OMX_IN OMX_COMMANDTYPE cmd,
        OMX_IN OMX_U32 param1,
        OMX_IN OMX_PTR cmdData
 )
{
 (void)hComp;
 (void)cmdData;

    OMX_ERRORTYPE eRet = OMX_ErrorNone;
    OMX_STATETYPE eState = (OMX_STATETYPE) param1;
 int bFlag = 1;

 if (cmd == OMX_CommandStateSet) {
  
  
  
 if (m_state == OMX_StateLoaded) {
 if (eState == OMX_StateIdle) {
 if (allocate_done() ||
 ( m_sInPortDef.bEnabled == OMX_FALSE && m_sOutPortDef.bEnabled == OMX_FALSE)) {
                    DEBUG_PRINT_LOW("OMXCORE-SM: Loaded-->Idle");
 } else {
                    DEBUG_PRINT_LOW("OMXCORE-SM: Loaded-->Idle-Pending");
                    BITMASK_SET(&m_flags, OMX_COMPONENT_IDLE_PENDING);
                    bFlag = 0;
 }
 }
  
 else if (eState == OMX_StateLoaded) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Loaded-->Loaded");
                post_event(OMX_EventError,OMX_ErrorSameState,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorSameState;
 }
  
 else if (eState == OMX_StateWaitForResources) {
  
                DEBUG_PRINT_LOW("OMXCORE-SM: Loaded-->WaitForResources");
 }
  
 else if (eState == OMX_StateExecuting) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Loaded-->Executing");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StatePause) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Loaded-->Pause");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StateInvalid) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Loaded-->Invalid");
                post_event(OMX_EventError,eState,OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorInvalidState;
 } else {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Loaded-->%d Not Handled",\
                        eState);
                eRet = OMX_ErrorBadParameter;
 }
 }

  
  
  
 else if (m_state == OMX_StateIdle) {
 if (eState == OMX_StateLoaded) {
 if (release_done()) {
  
                    DEBUG_PRINT_LOW("OMXCORE-SM: Idle-->Loaded");
 if (dev_stop() != 0) {
                        DEBUG_PRINT_ERROR("ERROR: dev_stop() failed at Idle --> Loaded");
                        eRet = OMX_ErrorHardware;
 }
 } else {
                    DEBUG_PRINT_LOW("OMXCORE-SM: Idle-->Loaded-Pending");
                    BITMASK_SET(&m_flags, OMX_COMPONENT_LOADING_PENDING);
                    bFlag = 0;
 }
 }
  
 else if (eState == OMX_StateExecuting) {
 if ( dev_start() ) {
                    DEBUG_PRINT_ERROR("ERROR: dev_start() failed in SCP on Idle --> Exe");
                    omx_report_error ();
                    eRet = OMX_ErrorHardware;
 } else {
                    BITMASK_SET(&m_flags,OMX_COMPONENT_EXECUTE_PENDING);
                    DEBUG_PRINT_LOW("OMXCORE-SM: Idle-->Executing");
                    bFlag = 0;
 }

                dev_start_done();
 }
  
 else if (eState == OMX_StateIdle) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Idle-->Idle");
                post_event(OMX_EventError,OMX_ErrorSameState,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorSameState;
 }
  
 else if (eState == OMX_StateWaitForResources) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Idle-->WaitForResources");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StatePause) {
  
 if ( dev_start() ) {
                    DEBUG_PRINT_ERROR("ERROR: dev_start() failed in SCP on Idle --> Pause");
                    omx_report_error ();
                    eRet = OMX_ErrorHardware;
 } else {
                    BITMASK_SET(&m_flags,OMX_COMPONENT_PAUSE_PENDING);
                    DEBUG_PRINT_LOW("OMXCORE-SM: Idle-->Pause");
                    bFlag = 0;
 }
 }
  
 else if (eState == OMX_StateInvalid) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Idle-->Invalid");
                post_event(OMX_EventError,eState,OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorInvalidState;
 } else {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Idle --> %d Not Handled",eState);
                eRet = OMX_ErrorBadParameter;
 }
 }

  
  
  
 else if (m_state == OMX_StateExecuting) {
  
 if (eState == OMX_StateIdle) {
  
                DEBUG_PRINT_LOW("OMXCORE-SM: Executing --> Idle");
                BITMASK_SET(&m_flags,OMX_COMPONENT_IDLE_PENDING);
                execute_omx_flush(OMX_ALL);
                bFlag = 0;
 }
  
 else if (eState == OMX_StatePause) {

 if (dev_pause()) {
                    DEBUG_PRINT_ERROR("ERROR: dev_pause() failed in SCP on Exe --> Pause");
                    post_event(OMX_EventError,OMX_ErrorHardware,\
                            OMX_COMPONENT_GENERATE_EVENT);
                    eRet = OMX_ErrorHardware;
 } else {
                    BITMASK_SET(&m_flags,OMX_COMPONENT_PAUSE_PENDING);
                    DEBUG_PRINT_LOW("OMXCORE-SM: Executing-->Pause");
                    bFlag = 0;
 }
 }
  
 else if (eState == OMX_StateLoaded) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Executing --> Loaded");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StateWaitForResources) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Executing --> WaitForResources");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StateExecuting) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Executing --> Executing");
                post_event(OMX_EventError,OMX_ErrorSameState,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorSameState;
 }
  
 else if (eState == OMX_StateInvalid) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Executing --> Invalid");
                post_event(OMX_EventError,eState,OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorInvalidState;
 } else {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Executing --> %d Not Handled",eState);
                eRet = OMX_ErrorBadParameter;
 }
 }
  
  
  
 else if (m_state == OMX_StatePause) {
  
 if (eState == OMX_StateExecuting) {
                DEBUG_PRINT_LOW("Pause --> Executing");
 if ( dev_resume() ) {
                    post_event(OMX_EventError,OMX_ErrorHardware,\
                            OMX_COMPONENT_GENERATE_EVENT);
                    eRet = OMX_ErrorHardware;
 } else {
                    BITMASK_SET(&m_flags,OMX_COMPONENT_EXECUTE_PENDING);
                    DEBUG_PRINT_LOW("OMXCORE-SM: Pause-->Executing");
                    post_event (0, 0, OMX_COMPONENT_GENERATE_RESUME_DONE);
                    bFlag = 0;
 }
 }
  
 else if (eState == OMX_StateIdle) {
  
                DEBUG_PRINT_LOW("Pause --> Idle");
                BITMASK_SET(&m_flags,OMX_COMPONENT_IDLE_PENDING);
                execute_omx_flush(OMX_ALL);
                bFlag = 0;
 }
  
 else if (eState == OMX_StateLoaded) {
                DEBUG_PRINT_ERROR("ERROR: Pause --> loaded");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StateWaitForResources) {
                DEBUG_PRINT_ERROR("ERROR: Pause --> WaitForResources");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StatePause) {
                DEBUG_PRINT_ERROR("ERROR: Pause --> Pause");
                post_event(OMX_EventError,OMX_ErrorSameState,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorSameState;
 }
  
 else if (eState == OMX_StateInvalid) {
                DEBUG_PRINT_ERROR("ERROR: Pause --> Invalid");
                post_event(OMX_EventError,eState,OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorInvalidState;
 } else {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Paused --> %d Not Handled",eState);
                eRet = OMX_ErrorBadParameter;
 }
 }
  
  
  
 else if (m_state == OMX_StateWaitForResources) {
  
 if (eState == OMX_StateLoaded) {
  
                DEBUG_PRINT_LOW("OMXCORE-SM: WaitForResources-->Loaded");
 }
  
 else if (eState == OMX_StateWaitForResources) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: WaitForResources-->WaitForResources");
                post_event(OMX_EventError,OMX_ErrorSameState,
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorSameState;
 }
  
 else if (eState == OMX_StateExecuting) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: WaitForResources-->Executing");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StatePause) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: WaitForResources-->Pause");
                post_event(OMX_EventError,OMX_ErrorIncorrectStateTransition,\
                        OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorIncorrectStateTransition;
 }
  
 else if (eState == OMX_StateInvalid) {
                DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: WaitForResources-->Invalid");
                post_event(OMX_EventError,eState,OMX_COMPONENT_GENERATE_EVENT);
                eRet = OMX_ErrorInvalidState;
 }
  

 } else {
            DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: %d --> %d(Not Handled)",m_state,eState);
            eRet = OMX_ErrorBadParameter;
 }
 }
  
  
  
 else if (m_state == OMX_StateInvalid) {
  
 if (eState == (OMX_StateLoaded || OMX_StateWaitForResources
 || OMX_StateIdle || OMX_StateExecuting
 || OMX_StatePause || OMX_StateInvalid)) {
            DEBUG_PRINT_ERROR("ERROR: OMXCORE-SM: Invalid -->Loaded");
            post_event(OMX_EventError,OMX_ErrorInvalidState,\
                    OMX_COMPONENT_GENERATE_EVENT);
            eRet = OMX_ErrorInvalidState;
 }
 } else if (cmd == OMX_CommandFlush) {
 if (0 == param1 || OMX_ALL == param1) {
            BITMASK_SET(&m_flags, OMX_COMPONENT_INPUT_FLUSH_PENDING);
 }
 if (1 == param1 || OMX_ALL == param1) {
            BITMASK_SET(&m_flags, OMX_COMPONENT_OUTPUT_FLUSH_PENDING);
 }

        execute_omx_flush(param1);
        bFlag = 0;
 } else if ( cmd == OMX_CommandPortEnable) {
 if (param1 == PORT_INDEX_IN || param1 == OMX_ALL) {
            m_sInPortDef.bEnabled = OMX_TRUE;

 if ( (m_state == OMX_StateLoaded &&
 !BITMASK_PRESENT(&m_flags,OMX_COMPONENT_IDLE_PENDING))
 || allocate_input_done()) {
                post_event(OMX_CommandPortEnable,PORT_INDEX_IN,
                        OMX_COMPONENT_GENERATE_EVENT);
 } else {
                DEBUG_PRINT_LOW("OMXCORE-SM: Disabled-->Enabled Pending");
                BITMASK_SET(&m_flags, OMX_COMPONENT_INPUT_ENABLE_PENDING);
                bFlag = 0;
 }
 }
 if (param1 == PORT_INDEX_OUT || param1 == OMX_ALL) {
            m_sOutPortDef.bEnabled = OMX_TRUE;

 if ( (m_state == OMX_StateLoaded &&
 !BITMASK_PRESENT(&m_flags,OMX_COMPONENT_IDLE_PENDING))
 || (allocate_output_done())) {
                post_event(OMX_CommandPortEnable,PORT_INDEX_OUT,
                        OMX_COMPONENT_GENERATE_EVENT);

 } else {
                DEBUG_PRINT_LOW("OMXCORE-SM: Disabled-->Enabled Pending");
                BITMASK_SET(&m_flags, OMX_COMPONENT_OUTPUT_ENABLE_PENDING);
                bFlag = 0;
 }
 }
 } else if (cmd == OMX_CommandPortDisable) {
 if (param1 == PORT_INDEX_IN || param1 == OMX_ALL) {
            m_sInPortDef.bEnabled = OMX_FALSE;
 if ((m_state == OMX_StateLoaded || m_state == OMX_StateIdle)
 && release_input_done()) {
                post_event(OMX_CommandPortDisable,PORT_INDEX_IN,
                        OMX_COMPONENT_GENERATE_EVENT);
 } else {
                BITMASK_SET(&m_flags, OMX_COMPONENT_INPUT_DISABLE_PENDING);
 if (m_state == OMX_StatePause ||m_state == OMX_StateExecuting) {
                    execute_omx_flush(PORT_INDEX_IN);
 }

                bFlag = 0;
 }
 }
 if (param1 == PORT_INDEX_OUT || param1 == OMX_ALL) {
            m_sOutPortDef.bEnabled = OMX_FALSE;

 if ((m_state == OMX_StateLoaded || m_state == OMX_StateIdle)
 && release_output_done()) {
                post_event(OMX_CommandPortDisable,PORT_INDEX_OUT,\
                        OMX_COMPONENT_GENERATE_EVENT);
 } else {
                BITMASK_SET(&m_flags, OMX_COMPONENT_OUTPUT_DISABLE_PENDING);
 if (m_state == OMX_StatePause ||m_state == OMX_StateExecuting) {
                    execute_omx_flush(PORT_INDEX_OUT);
 }
                bFlag = 0;

 }
 }
 } else {
        DEBUG_PRINT_ERROR("ERROR: Invalid Command received other than StateSet (%d)",cmd);
        eRet = OMX_ErrorNotImplemented;
 }
 if (eRet == OMX_ErrorNone && bFlag) {
        post_event(cmd,eState,OMX_COMPONENT_GENERATE_EVENT);
 }
    sem_post(&m_cmd_lock);
 return eRet;
}
