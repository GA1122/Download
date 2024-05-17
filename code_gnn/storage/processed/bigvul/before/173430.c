OMX_ERRORTYPE omx_vdec::empty_this_buffer_proxy_arbitrary (
        OMX_HANDLETYPE hComp,
        OMX_BUFFERHEADERTYPE *buffer
 )
{
 unsigned address,p2,id;
    DEBUG_PRINT_LOW("Empty this arbitrary");

 if (buffer == NULL) {
 return OMX_ErrorBadParameter;
 }
    DEBUG_PRINT_LOW("ETBProxyArb: bufhdr = %p, bufhdr->pBuffer = %p", buffer, buffer->pBuffer);
    DEBUG_PRINT_LOW("ETBProxyArb: nFilledLen %u, flags %u, timestamp %lld",
 (unsigned int)buffer->nFilledLen, (unsigned int)buffer->nFlags, buffer->nTimeStamp);

  
  
 if ((input_flush_progress == true) || ((buffer->nFilledLen == 0) &&
 ((buffer->nFlags & OMX_BUFFERFLAG_EOS) == 0))) {
        DEBUG_PRINT_HIGH("return zero legth buffer or flush in progress");
        m_cb.EmptyBufferDone (hComp,m_app_data,buffer);
 return OMX_ErrorNone;
 }

 if (psource_frame == NULL) {
        DEBUG_PRINT_LOW("Set Buffer as source Buffer %p time stamp %lld",buffer,buffer->nTimeStamp);
        psource_frame = buffer;
        DEBUG_PRINT_LOW("Try to Push One Input Buffer ");
        push_input_buffer (hComp);
 } else {
        DEBUG_PRINT_LOW("Push the source buffer into pendingq %p",buffer);
 if (!m_input_pending_q.insert_entry((unsigned long)buffer, (unsigned)NULL,
 (unsigned)NULL)) {
 return OMX_ErrorBadParameter;
 }
 }

 if (codec_config_flag && !(buffer->nFlags & OMX_BUFFERFLAG_CODECCONFIG)) {
        codec_config_flag = false;
 }
 return OMX_ErrorNone;
}
