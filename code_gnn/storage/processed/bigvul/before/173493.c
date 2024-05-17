void omx_vdec::send_codec_config() {
 if (codec_config_flag) {
 unsigned long p1 = 0;  
 unsigned long p2 = 0;  
 unsigned long ident = 0;
        pthread_mutex_lock(&m_lock);
        DEBUG_PRINT_LOW("\n Check Queue for codec_config buffer \n");
 while (m_etb_q.m_size) {
            m_etb_q.pop_entry(&p1,&p2,&ident);
 if (ident == OMX_COMPONENT_GENERATE_ETB_ARBITRARY) {
 if (((OMX_BUFFERHEADERTYPE *)p2)->nFlags & OMX_BUFFERFLAG_CODECCONFIG) {
 if (empty_this_buffer_proxy_arbitrary((OMX_HANDLETYPE)p1,\
 (OMX_BUFFERHEADERTYPE *)p2) != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("\n empty_this_buffer_proxy_arbitrary failure");
                        omx_report_error();
 }
 } else {
                    DEBUG_PRINT_LOW("\n Flush Input Heap Buffer %p",(OMX_BUFFERHEADERTYPE *)p2);
                    m_cb.EmptyBufferDone(&m_cmp ,m_app_data, (OMX_BUFFERHEADERTYPE *)p2);
 }
 } else if (ident == OMX_COMPONENT_GENERATE_ETB) {
 if (((OMX_BUFFERHEADERTYPE *)p2)->nFlags & OMX_BUFFERFLAG_CODECCONFIG) {
 if (empty_this_buffer_proxy((OMX_HANDLETYPE)p1,\
 (OMX_BUFFERHEADERTYPE *)p2) != OMX_ErrorNone) {
                        DEBUG_PRINT_ERROR("\n empty_this_buffer_proxy failure");
                        omx_report_error ();
 }
 } else {
                    pending_input_buffers++;
                    DEBUG_PRINT_LOW("\n Flush Input OMX_COMPONENT_GENERATE_ETB %p, pending_input_buffers %d",
 (OMX_BUFFERHEADERTYPE *)p2, pending_input_buffers);
                    empty_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p2);
 }
 } else if (ident == OMX_COMPONENT_GENERATE_EBD) {
                DEBUG_PRINT_LOW("\n Flush Input OMX_COMPONENT_GENERATE_EBD %p",
 (OMX_BUFFERHEADERTYPE *)p1);
                empty_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p1);
 }
 }
        pthread_mutex_unlock(&m_lock);
 }
}
