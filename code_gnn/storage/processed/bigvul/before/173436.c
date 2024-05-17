bool omx_vdec::execute_output_flush()
{
 unsigned long p1 = 0;  
 unsigned long p2 = 0;  
 unsigned long ident = 0;
 bool bRet = true;

  
    pthread_mutex_lock(&m_lock);
    DEBUG_PRINT_LOW("Initiate Output Flush");

 if(m_last_rendered_TS > 0) {
        m_last_rendered_TS = 0;
 }

 while (m_ftb_q.m_size) {
        DEBUG_PRINT_LOW("Buffer queue size %lu pending buf cnt %d",
                m_ftb_q.m_size,pending_output_buffers);
        m_ftb_q.pop_entry(&p1,&p2,&ident);
        DEBUG_PRINT_LOW("ID(%lx) P1(%lx) P2(%lx)", ident, p1, p2);
 if (ident == m_fill_output_msg ) {
            m_cb.FillBufferDone(&m_cmp, m_app_data, (OMX_BUFFERHEADERTYPE *)(intptr_t)p2);
 } else if (ident == OMX_COMPONENT_GENERATE_FBD) {
            fill_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)(intptr_t)p1);
 }
 }
    pthread_mutex_unlock(&m_lock);
    output_flush_progress = false;

 if (arbitrary_bytes) {
        prev_ts = LLONG_MAX;
        rst_prev_ts = true;
 }
    DEBUG_PRINT_HIGH("OMX flush o/p Port complete PenBuf(%d)", pending_output_buffers);
 return bRet;
}
