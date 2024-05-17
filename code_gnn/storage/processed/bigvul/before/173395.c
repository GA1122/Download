OMX_ERRORTYPE  omx_vdec::allocate_buffer(OMX_IN OMX_HANDLETYPE                hComp,
        OMX_INOUT OMX_BUFFERHEADERTYPE** bufferHdr,
        OMX_IN OMX_U32                        port,
        OMX_IN OMX_PTR                     appData,
        OMX_IN OMX_U32                       bytes)
{
 unsigned i = 0;
    OMX_ERRORTYPE eRet = OMX_ErrorNone;  

    DEBUG_PRINT_LOW("Allocate buffer on port %d", (int)port);
 if (m_state == OMX_StateInvalid) {
        DEBUG_PRINT_ERROR("Allocate Buf in Invalid State");
 return OMX_ErrorInvalidState;
 }

 if (port == OMX_CORE_INPUT_PORT_INDEX) {
 if (arbitrary_bytes) {
            eRet = allocate_input_heap_buffer (hComp,bufferHdr,port,appData,bytes);
 } else {
            eRet = allocate_input_buffer(hComp,bufferHdr,port,appData,bytes);
 }
 } else if (port == OMX_CORE_OUTPUT_PORT_INDEX) {
        eRet = client_buffers.allocate_buffers_color_convert(hComp,bufferHdr,port,
                appData,bytes);
 } else {
        DEBUG_PRINT_ERROR("Error: Invalid Port Index received %d",(int)port);
        eRet = OMX_ErrorBadPortIndex;
 }
    DEBUG_PRINT_LOW("Checking for Output Allocate buffer Done");
 if (eRet == OMX_ErrorNone) {
 if (allocate_done()) {
 if (BITMASK_PRESENT(&m_flags,OMX_COMPONENT_IDLE_PENDING)) {
                BITMASK_CLEAR((&m_flags),OMX_COMPONENT_IDLE_PENDING);
                post_event(OMX_CommandStateSet,OMX_StateIdle,
                        OMX_COMPONENT_GENERATE_EVENT);
 }
 }
 if (port == OMX_CORE_INPUT_PORT_INDEX && m_inp_bPopulated) {
 if (BITMASK_PRESENT(&m_flags,OMX_COMPONENT_INPUT_ENABLE_PENDING)) {
                BITMASK_CLEAR((&m_flags),OMX_COMPONENT_INPUT_ENABLE_PENDING);
                post_event(OMX_CommandPortEnable,
                        OMX_CORE_INPUT_PORT_INDEX,
                        OMX_COMPONENT_GENERATE_EVENT);
 }
 }
 if (port == OMX_CORE_OUTPUT_PORT_INDEX && m_out_bPopulated) {
 if (BITMASK_PRESENT(&m_flags,OMX_COMPONENT_OUTPUT_ENABLE_PENDING)) {
                BITMASK_CLEAR((&m_flags),OMX_COMPONENT_OUTPUT_ENABLE_PENDING);
                post_event(OMX_CommandPortEnable,
                        OMX_CORE_OUTPUT_PORT_INDEX,
                        OMX_COMPONENT_GENERATE_EVENT);
 }
 }
 }
    DEBUG_PRINT_LOW("Allocate Buffer exit with ret Code %d",eRet);
 return eRet;
}
