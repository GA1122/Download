bool omx_video::execute_output_flush(void)
{
 unsigned long p1 = 0;  
 unsigned long p2 = 0;  
 unsigned long ident = 0;
 bool bRet = true;

  
    DEBUG_PRINT_LOW("execute_output_flush");
    pthread_mutex_lock(&m_lock);
 while (m_ftb_q.m_size) {
        m_ftb_q.pop_entry(&p1,&p2,&ident);

 if (ident == OMX_COMPONENT_GENERATE_FTB ) {
            pending_output_buffers++;
            fill_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p2);
 } else if (ident == OMX_COMPONENT_GENERATE_FBD) {
            fill_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p1);
 }
 }

    pthread_mutex_unlock(&m_lock);
  
 if (dev_flush(PORT_INDEX_OUT)) {
        DEBUG_PRINT_ERROR("ERROR: o/p dev_flush() Failed");
 return false;
 }

 return bRet;
}
