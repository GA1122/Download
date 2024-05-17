OMX_ERRORTYPE  omx_vdec::send_command(OMX_IN OMX_HANDLETYPE hComp,
        OMX_IN OMX_COMMANDTYPE cmd,
        OMX_IN OMX_U32 param1,
        OMX_IN OMX_PTR cmdData
 )
{
 (void) hComp;
 (void) cmdData;
    DEBUG_PRINT_LOW("send_command: Recieved a Command from Client");
 if (m_state == OMX_StateInvalid) {
        DEBUG_PRINT_ERROR("ERROR: Send Command in Invalid State");
 return OMX_ErrorInvalidState;
 }
 if (cmd == OMX_CommandFlush && param1 != OMX_CORE_INPUT_PORT_INDEX
 && param1 != OMX_CORE_OUTPUT_PORT_INDEX && param1 != OMX_ALL) {
        DEBUG_PRINT_ERROR("send_command(): ERROR OMX_CommandFlush "
 "to invalid port: %u", (unsigned int)param1);
 return OMX_ErrorBadPortIndex;
 }

    post_event((unsigned)cmd,(unsigned)param1,OMX_COMPONENT_GENERATE_COMMAND);
    sem_wait(&m_cmd_lock);
    DEBUG_PRINT_LOW("send_command: Command Processed");
 return OMX_ErrorNone;
}
