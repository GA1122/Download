OMX_ERRORTYPE  omx_video::fill_this_buffer_proxy(
        OMX_IN OMX_HANDLETYPE        hComp,
        OMX_IN OMX_BUFFERHEADERTYPE* bufferAdd)
{
 (void)hComp;
    OMX_U8 *pmem_data_buf = NULL;
    OMX_ERRORTYPE nRet = OMX_ErrorNone;

    DEBUG_PRINT_LOW("FTBProxy: bufferAdd->pBuffer[%p]", bufferAdd->pBuffer);

 if (bufferAdd == NULL || ((bufferAdd - m_out_mem_ptr) >= (int)m_sOutPortDef.nBufferCountActual) ) {
        DEBUG_PRINT_ERROR("ERROR: FTBProxy: Invalid i/p params");
 return OMX_ErrorBadParameter;
 }

    pending_output_buffers++;
  
 if ( m_sOutPortDef.bEnabled != OMX_TRUE || output_flush_progress == true) {
        DEBUG_PRINT_LOW("o/p port is Disabled or Flush in Progress");
        post_event ((unsigned long)bufferAdd,0,
                OMX_COMPONENT_GENERATE_FBD);
 return OMX_ErrorNone;
 }

 if (output_use_buffer && !m_use_output_pmem) {
        DEBUG_PRINT_LOW("Heap UseBuffer case");
        pmem_data_buf = (OMX_U8 *)m_pOutput_pmem[bufferAdd - m_out_mem_ptr].buffer;
 }

 if (dev_fill_buf(bufferAdd, pmem_data_buf,(bufferAdd - m_out_mem_ptr),m_pOutput_pmem[bufferAdd - m_out_mem_ptr].fd) != true) {
        DEBUG_PRINT_ERROR("ERROR: dev_fill_buf() Failed");
        post_event ((unsigned long)bufferAdd,0,OMX_COMPONENT_GENERATE_FBD);
        pending_output_buffers--;
 return OMX_ErrorBadParameter;
 }

 return OMX_ErrorNone;
}
