bool omx_vdec::execute_input_flush()
{
 unsigned       i =0;
 unsigned long p1 = 0;  
 unsigned long p2 = 0;  
 unsigned long ident = 0;
 bool bRet = true;

  
    DEBUG_PRINT_LOW("Initiate Input Flush");

    pthread_mutex_lock(&m_lock);
    DEBUG_PRINT_LOW("Check if the Queue is empty");
 while (m_etb_q.m_size) {
        m_etb_q.pop_entry(&p1,&p2,&ident);

 if (ident == OMX_COMPONENT_GENERATE_ETB_ARBITRARY) {
            DEBUG_PRINT_LOW("Flush Input Heap Buffer %p",(OMX_BUFFERHEADERTYPE *)p2);
            m_cb.EmptyBufferDone(&m_cmp ,m_app_data, (OMX_BUFFERHEADERTYPE *)p2);
 } else if (ident == OMX_COMPONENT_GENERATE_ETB) {
            pending_input_buffers++;
            DEBUG_PRINT_LOW("Flush Input OMX_COMPONENT_GENERATE_ETB %p, pending_input_buffers %d",
 (OMX_BUFFERHEADERTYPE *)p2, pending_input_buffers);
            empty_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p2);
 } else if (ident == OMX_COMPONENT_GENERATE_EBD) {
            DEBUG_PRINT_LOW("Flush Input OMX_COMPONENT_GENERATE_EBD %p",
 (OMX_BUFFERHEADERTYPE *)p1);
            empty_buffer_done(&m_cmp,(OMX_BUFFERHEADERTYPE *)p1);
 }
 }
    time_stamp_dts.flush_timestamp();
  
 if (arbitrary_bytes && !(codec_config_flag)) {
        DEBUG_PRINT_LOW("Reset all the variables before flusing");
        h264_scratch.nFilledLen = 0;
        nal_count = 0;
        look_ahead_nal = false;
        frame_count = 0;
        h264_last_au_ts = LLONG_MAX;
        h264_last_au_flags = 0;
        memset(m_demux_offsets, 0, ( sizeof(OMX_U32) * 8192) );
        m_demux_entries = 0;
        DEBUG_PRINT_LOW("Initialize parser");
 if (m_frame_parser.mutils) {
            m_frame_parser.mutils->initialize_frame_checking_environment();
 }

 while (m_input_pending_q.m_size) {
            m_input_pending_q.pop_entry(&p1,&p2,&ident);
            m_cb.EmptyBufferDone(&m_cmp ,m_app_data, (OMX_BUFFERHEADERTYPE *)p1);
 }

 if (psource_frame) {
            m_cb.EmptyBufferDone(&m_cmp ,m_app_data,psource_frame);
            psource_frame = NULL;
 }

 if (pdest_frame) {
            pdest_frame->nFilledLen = 0;
            m_input_free_q.insert_entry((unsigned long) pdest_frame, (unsigned int)NULL,
 (unsigned int)NULL);
            pdest_frame = NULL;
 }
        m_frame_parser.flush();
 } else if (codec_config_flag) {
        DEBUG_PRINT_HIGH("frame_parser flushing skipped due to codec config buffer "
 "is not sent to the driver yet");
 }
    pthread_mutex_unlock(&m_lock);
    input_flush_progress = false;
 if (!arbitrary_bytes) {
        prev_ts = LLONG_MAX;
        rst_prev_ts = true;
 }
#ifdef _ANDROID_
 if (m_debug_timestamp) {
        m_timestamp_list.reset_ts_list();
 }
#endif
    DEBUG_PRINT_HIGH("OMX flush i/p Port complete PenBuf(%d)", pending_input_buffers);
 return bRet;
}
