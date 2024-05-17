void omx_video::complete_pending_buffer_done_cbs()
{
 unsigned long p1;
 unsigned long p2;
 unsigned long ident;
    omx_cmd_queue tmp_q, pending_bd_q;
    pthread_mutex_lock(&m_lock);
 while (m_ftb_q.m_size) {
        m_ftb_q.pop_entry(&p1,&p2,&ident);
 if (ident == OMX_COMPONENT_GENERATE_FBD) {
            pending_bd_q.insert_entry(p1,p2,ident);
 } else {
            tmp_q.insert_entry(p1,p2,ident);
 }
 }
 while (tmp_q.m_size) {
        tmp_q.pop_entry(&p1,&p2,&ident);
        m_ftb_q.insert_entry(p1,p2,ident);
 }
 while (m_etb_q.m_size) {
        m_etb_q.pop_entry(&p1,&p2,&ident);
 if (ident == OMX_COMPONENT_GENERATE_EBD) {
            pending_bd_q.insert_entry(p1,p2,ident);
 } else {
            tmp_q.insert_entry(p1,p2,ident);
 }
 }
 while (tmp_q.m_size) {
        tmp_q.pop_entry(&p1,&p2,&ident);
        m_etb_q.insert_entry(p1,p2,ident);
 }
    pthread_mutex_unlock(&m_lock);
 while (pending_bd_q.m_size) {
        pending_bd_q.pop_entry(&p1,&p2,&ident);
 switch (ident) {
 case OMX_COMPONENT_GENERATE_EBD:
 if (empty_buffer_done(&m_cmp, (OMX_BUFFERHEADERTYPE *)p1) != OMX_ErrorNone) {
                    DEBUG_PRINT_ERROR("ERROR: empty_buffer_done() failed!");
                    omx_report_error ();
 }
 break;

 case OMX_COMPONENT_GENERATE_FBD:
 if (fill_buffer_done(&m_cmp, (OMX_BUFFERHEADERTYPE *)p1) != OMX_ErrorNone ) {
                    DEBUG_PRINT_ERROR("ERROR: fill_buffer_done() failed!");
                    omx_report_error ();
 }
 break;
 }
 }
}
