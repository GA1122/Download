OMX_ERRORTYPE omx_video::empty_buffer_done(OMX_HANDLETYPE         hComp,
        OMX_BUFFERHEADERTYPE* buffer)
{
 int buffer_index  = -1;

    buffer_index = buffer - ((mUseProxyColorFormat && !mUsesColorConversion) ? meta_buffer_hdr : m_inp_mem_ptr);
    DEBUG_PRINT_LOW("empty_buffer_done: buffer[%p]", buffer);
 if (buffer == NULL ||
 ((buffer_index > (int)m_sInPortDef.nBufferCountActual))) {
        DEBUG_PRINT_ERROR("ERROR in empty_buffer_done due to index buffer");
 return OMX_ErrorBadParameter;
 }

    pending_input_buffers--;

 if (mUseProxyColorFormat &&
 (buffer_index >= 0 && (buffer_index < (int)m_sInPortDef.nBufferCountActual))) {
 if (!pdest_frame  && !input_flush_progress && mUsesColorConversion) {
            pdest_frame = buffer;
            DEBUG_PRINT_LOW("empty_buffer_done pdest_frame address is %p",pdest_frame);
 return push_input_buffer(hComp);
 }
 bool handleEmptyEosBuffer = (mEmptyEosBuffer == buffer);
 if (mUsesColorConversion || handleEmptyEosBuffer) {
 if (handleEmptyEosBuffer) {
                mEmptyEosBuffer = NULL;
 }
            DEBUG_PRINT_LOW("empty_buffer_done insert address is %p",buffer);
 if (!m_opq_pmem_q.insert_entry((unsigned long)buffer, 0, 0)) {
                DEBUG_PRINT_ERROR("empty_buffer_done: pmem queue is full");
 return OMX_ErrorBadParameter;
 }
 } else {
 if (m_pCallbacks.EmptyBufferDone && buffer) {
                m_pCallbacks.EmptyBufferDone(hComp, m_app_data, buffer);
                DEBUG_PRINT_LOW("empty_buffer_done: Returning client buf %p", buffer);
 }
 }
 } else if (m_pCallbacks.EmptyBufferDone) {
        m_pCallbacks.EmptyBufferDone(hComp ,m_app_data, buffer);
 }
 return OMX_ErrorNone;
}
