bool omx_video::execute_input_flush(void)
{
 unsigned long p1 = 0;  
 unsigned long p2 = 0;  
 unsigned long ident = 0;
 bool bRet = true;

  
    DEBUG_PRINT_LOW("execute_input_flush");

    pthread_mutex_lock(&m_lock);
 while (m_etb_q.m_size) {
        m_etb_q.pop_entry(&p1,&p2,&ident);
 if (ident == OMX_COMPONENT_GENERATE_ETB) {
            pending_input_buffers++;
            empty_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p2);
 } else if (ident == OMX_COMPONENT_GENERATE_EBD) {
            empty_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p1);
 } else if (ident == OMX_COMPONENT_GENERATE_ETB_OPQ) {
            m_pCallbacks.EmptyBufferDone(&m_cmp,m_app_data,(OMX_BUFFERHEADERTYPE *)p2);
 }
 }
 if (mUseProxyColorFormat) {
 if (psource_frame) {
            m_pCallbacks.EmptyBufferDone(&m_cmp,m_app_data,psource_frame);
            psource_frame = NULL;
 }
 while (m_opq_meta_q.m_size) {
 unsigned long p1,p2,id;
            m_opq_meta_q.pop_entry(&p1,&p2,&id);
            m_pCallbacks.EmptyBufferDone(&m_cmp,m_app_data,
 (OMX_BUFFERHEADERTYPE  *)p1);
 }
 if (pdest_frame) {
            m_opq_pmem_q.insert_entry((unsigned long)pdest_frame,0,0);
            pdest_frame = NULL;
 }
 }
    pthread_mutex_unlock(&m_lock);
  
 if (dev_flush(PORT_INDEX_IN)) {
        DEBUG_PRINT_ERROR("ERROR: i/p dev_flush() Failed");
 return false;
 }

 return bRet;
}
