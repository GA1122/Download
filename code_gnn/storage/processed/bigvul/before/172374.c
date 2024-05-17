OMX_ERRORTYPE  omx_video::send_command(OMX_IN OMX_HANDLETYPE hComp,
        OMX_IN OMX_COMMANDTYPE cmd,
        OMX_IN OMX_U32 param1,
        OMX_IN OMX_PTR cmdData
 )
{
 (void)hComp;
 if (m_state == OMX_StateInvalid) {
        DEBUG_PRINT_ERROR("ERROR: Send Command in Invalid State");
 return OMX_ErrorInvalidState;
 }

 if (cmd == OMX_CommandFlush || cmd == OMX_CommandPortDisable || cmd == OMX_CommandPortEnable) {
 if ((param1 != (OMX_U32)PORT_INDEX_IN) && (param1 != (OMX_U32)PORT_INDEX_OUT) && (param1 != (OMX_U32)PORT_INDEX_BOTH)) {
            DEBUG_PRINT_ERROR("ERROR: omx_video::send_command-->bad port index");
 return OMX_ErrorBadPortIndex;
 }
 }
 if (cmd == OMX_CommandMarkBuffer) {
 if (param1 != PORT_INDEX_IN) {
            DEBUG_PRINT_ERROR("ERROR: omx_video::send_command-->bad port index");
 return OMX_ErrorBadPortIndex;
 }
 if (!cmdData) {
            DEBUG_PRINT_ERROR("ERROR: omx_video::send_command-->param is null");
 return OMX_ErrorBadParameter;
 }
 }

    post_event((unsigned long)cmd,(unsigned long)param1,OMX_COMPONENT_GENERATE_COMMAND);
    sem_wait(&m_cmd_lock);
 return OMX_ErrorNone;
}
