OMX_ERRORTYPE omx_video::fill_buffer_done(OMX_HANDLETYPE hComp,
        OMX_BUFFERHEADERTYPE * buffer)
{
#ifdef _MSM8974_
 int index = buffer - m_out_mem_ptr;
#endif
    DEBUG_PRINT_LOW("fill_buffer_done: buffer->pBuffer[%p], flags=0x%x size = %u",
            buffer->pBuffer, (unsigned)buffer->nFlags, (unsigned int)buffer->nFilledLen);
 if (buffer == NULL || ((buffer - m_out_mem_ptr) > (int)m_sOutPortDef.nBufferCountActual)) {
 return OMX_ErrorBadParameter;
 }

    pending_output_buffers--;

 if(!secure_session) {
    extra_data_handle.create_extra_data(buffer);
#ifndef _MSM8974_
 if (buffer->nFlags & OMX_BUFFERFLAG_EXTRADATA) {
        DEBUG_PRINT_LOW("parsing extradata");
        extra_data_handle.parse_extra_data(buffer);
 }
#endif
 }

  
 if (m_pCallbacks.FillBufferDone) {
 if (buffer->nFilledLen > 0) {
            m_fbd_count++;

 if (dev_get_output_log_flag()) {
                dev_output_log_buffers((const char*)buffer->pBuffer, buffer->nFilledLen);
 }
 }
#ifdef _MSM8974_
 if (buffer->nFlags & OMX_BUFFERFLAG_EXTRADATA) {
 if (!dev_handle_extradata((void *)buffer, index))
                DEBUG_PRINT_ERROR("Failed to parse extradata");

            dev_extradata_log_buffers((char *)(((unsigned long)buffer->pBuffer + buffer->nOffset +
                        buffer->nFilledLen + 3) & (~3)));
 }
#endif
        m_pCallbacks.FillBufferDone (hComp,m_app_data,buffer);
 } else {
 return OMX_ErrorBadParameter;
 }
 return OMX_ErrorNone;
}
