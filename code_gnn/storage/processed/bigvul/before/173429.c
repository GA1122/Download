OMX_ERRORTYPE omx_vdec::empty_buffer_done(OMX_HANDLETYPE         hComp,
        OMX_BUFFERHEADERTYPE* buffer)
{

 if (buffer == NULL || ((buffer - m_inp_mem_ptr) > (int)drv_ctx.ip_buf.actualcount)) {
        DEBUG_PRINT_ERROR("empty_buffer_done: ERROR bufhdr = %p", buffer);
 return OMX_ErrorBadParameter;
 }

    DEBUG_PRINT_LOW("empty_buffer_done: bufhdr = %p, bufhdr->pBuffer = %p, bufhdr->nFlags = %x",
            buffer, buffer->pBuffer, buffer->nFlags);
    pending_input_buffers--;

 if (arbitrary_bytes) {
 if (pdest_frame == NULL && input_flush_progress == false) {
            DEBUG_PRINT_LOW("Push input from buffer done address of Buffer %p",buffer);
            pdest_frame = buffer;
            buffer->nFilledLen = 0;
            buffer->nTimeStamp = LLONG_MAX;
            push_input_buffer (hComp);
 } else {
            DEBUG_PRINT_LOW("Push buffer into freeq address of Buffer %p",buffer);
            buffer->nFilledLen = 0;
 if (!m_input_free_q.insert_entry((unsigned long)buffer,
 (unsigned)NULL, (unsigned)NULL)) {
                DEBUG_PRINT_ERROR("ERROR:i/p free Queue is FULL Error");
 }
 }
 } else if (m_cb.EmptyBufferDone) {
        buffer->nFilledLen = 0;
 if (input_use_buffer == true) {
            buffer = &m_inp_heap_ptr[buffer-m_inp_mem_ptr];
 }
        m_cb.EmptyBufferDone(hComp ,m_app_data, buffer);
 }
 return OMX_ErrorNone;
}
