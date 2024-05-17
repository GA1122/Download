OMX_ERRORTYPE  omx_video::use_buffer(
        OMX_IN OMX_HANDLETYPE            hComp,
        OMX_INOUT OMX_BUFFERHEADERTYPE** bufferHdr,
        OMX_IN OMX_U32                   port,
        OMX_IN OMX_PTR                   appData,
        OMX_IN OMX_U32                   bytes,
        OMX_IN OMX_U8*                   buffer)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 if (m_state == OMX_StateInvalid) {
        DEBUG_PRINT_ERROR("ERROR: Use Buffer in Invalid State");
 return OMX_ErrorInvalidState;
 }
 if (port == PORT_INDEX_IN) {
        eRet = use_input_buffer(hComp,bufferHdr,port,appData,bytes,buffer);
 } else if (port == PORT_INDEX_OUT) {
        eRet = use_output_buffer(hComp,bufferHdr,port,appData,bytes,buffer);
 } else {
        DEBUG_PRINT_ERROR("ERROR: Invalid Port Index received %d",(int)port);
        eRet = OMX_ErrorBadPortIndex;
 }

 if (eRet == OMX_ErrorNone) {
 if (allocate_done()) {
 if (BITMASK_PRESENT(&m_flags,OMX_COMPONENT_IDLE_PENDING)) {
                BITMASK_CLEAR((&m_flags),OMX_COMPONENT_IDLE_PENDING);
                post_event(OMX_CommandStateSet,OMX_StateIdle,
                        OMX_COMPONENT_GENERATE_EVENT);
 }
 }
 if (port == PORT_INDEX_IN && m_sInPortDef.bPopulated) {
 if (BITMASK_PRESENT(&m_flags,OMX_COMPONENT_INPUT_ENABLE_PENDING)) {
                BITMASK_CLEAR((&m_flags),OMX_COMPONENT_INPUT_ENABLE_PENDING);
                post_event(OMX_CommandPortEnable,
                        PORT_INDEX_IN,
                        OMX_COMPONENT_GENERATE_EVENT);
 }

 } else if (port == PORT_INDEX_OUT && m_sOutPortDef.bPopulated) {
 if (BITMASK_PRESENT(&m_flags,OMX_COMPONENT_OUTPUT_ENABLE_PENDING)) {
                BITMASK_CLEAR((&m_flags),OMX_COMPONENT_OUTPUT_ENABLE_PENDING);
                post_event(OMX_CommandPortEnable,
                        PORT_INDEX_OUT,
                        OMX_COMPONENT_GENERATE_EVENT);
                m_event_port_settings_sent = false;
 }
 }
 }
 return eRet;
}
