OMX_ERRORTYPE omx_vdec::fill_buffer_done(OMX_HANDLETYPE hComp,
        OMX_BUFFERHEADERTYPE * buffer)
{
    OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO *pPMEMInfo = NULL;
 if (!buffer || (buffer - m_out_mem_ptr) >= (int)drv_ctx.op_buf.actualcount) {
        DEBUG_PRINT_ERROR("[FBD] ERROR in ptr(%p)", buffer);
 return OMX_ErrorBadParameter;
 } else if (output_flush_progress) {
        DEBUG_PRINT_LOW("FBD: Buffer (%p) flushed", buffer);
        buffer->nFilledLen = 0;
        buffer->nTimeStamp = 0;
        buffer->nFlags &= ~OMX_BUFFERFLAG_EXTRADATA;
        buffer->nFlags &= ~QOMX_VIDEO_BUFFERFLAG_EOSEQ;
        buffer->nFlags &= ~OMX_BUFFERFLAG_DATACORRUPT;
 }

 if (m_debug_extradata) {
 if (buffer->nFlags & QOMX_VIDEO_BUFFERFLAG_EOSEQ) {
            DEBUG_PRINT_HIGH("***************************************************");
            DEBUG_PRINT_HIGH("FillBufferDone: End Of Sequence Received");
            DEBUG_PRINT_HIGH("***************************************************");
 }

 if (buffer->nFlags & OMX_BUFFERFLAG_DATACORRUPT) {
            DEBUG_PRINT_HIGH("***************************************************");
            DEBUG_PRINT_HIGH("FillBufferDone: OMX_BUFFERFLAG_DATACORRUPT Received");
            DEBUG_PRINT_HIGH("***************************************************");
 }
 }


    DEBUG_PRINT_LOW("fill_buffer_done: bufhdr = %p, bufhdr->pBuffer = %p",
            buffer, buffer->pBuffer);
    pending_output_buffers --;

 if (buffer->nFlags & OMX_BUFFERFLAG_EOS) {
        DEBUG_PRINT_HIGH("Output EOS has been reached");
 if (!output_flush_progress)
            post_event((unsigned)NULL, (unsigned)NULL,
                    OMX_COMPONENT_GENERATE_EOS_DONE);

 if (psource_frame) {
            m_cb.EmptyBufferDone(&m_cmp, m_app_data, psource_frame);
            psource_frame = NULL;
 }
 if (pdest_frame) {
            pdest_frame->nFilledLen = 0;
            m_input_free_q.insert_entry((unsigned long) pdest_frame,(unsigned)NULL,
 (unsigned)NULL);
            pdest_frame = NULL;
 }
 }

 if (!output_flush_progress && (buffer->nFilledLen > 0)) {
        DEBUG_PRINT_LOW("Processing extradata");
        handle_extradata(buffer);
 }

#ifdef OUTPUT_EXTRADATA_LOG
 if (outputExtradataFile) {
 int buf_index = buffer - m_out_mem_ptr;
        OMX_U8 *pBuffer = (OMX_U8 *)(drv_ctx.ptr_outputbuffer[buf_index].bufferaddr);

        OMX_OTHER_EXTRADATATYPE *p_extra = NULL;
        p_extra = (OMX_OTHER_EXTRADATATYPE *)
 ((unsigned long)(pBuffer + buffer->nOffset + buffer->nFilledLen + 3)&(~3));

 while (p_extra && (OMX_U8*)p_extra < (pBuffer + buffer->nAllocLen) ) {
            DEBUG_PRINT_LOW("WRITING extradata, size=%d,type=%x",
                                    p_extra->nSize, p_extra->eType);
            fwrite (p_extra,1,p_extra->nSize,outputExtradataFile);

 if (p_extra->eType == OMX_ExtraDataNone) {
 break;
 }
            p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) + p_extra->nSize);
 }
 }
#endif

  
 if (!output_flush_progress) {
  
 bool is_duplicate_ts_valid = true;
 bool is_interlaced = (drv_ctx.interlace != VDEC_InterlaceFrameProgressive);

 if (output_capability == V4L2_PIX_FMT_MPEG4 ||
                output_capability == V4L2_PIX_FMT_MPEG2 ||
                output_capability == V4L2_PIX_FMT_DIVX ||
                output_capability == V4L2_PIX_FMT_DIVX_311)
            is_duplicate_ts_valid = false;

 if ((output_capability == V4L2_PIX_FMT_H264 ||
                output_capability == V4L2_PIX_FMT_H264_MVC) &&
                is_interlaced) {
 if (buffer->nFlags & QOMX_VIDEO_BUFFERFLAG_MBAFF) {
                is_interlaced = false;
 }
 }

 if (buffer->nFilledLen > 0) {
            time_stamp_dts.get_next_timestamp(buffer,
                    is_interlaced && is_duplicate_ts_valid);
 if (m_debug_timestamp) {
 {
                    OMX_TICKS expected_ts = 0;
                    m_timestamp_list.pop_min_ts(expected_ts);
 if (is_interlaced && is_duplicate_ts_valid) {
                        m_timestamp_list.pop_min_ts(expected_ts);
 }
                    DEBUG_PRINT_LOW("Current timestamp (%lld),Popped TIMESTAMP (%lld) from list",
                            buffer->nTimeStamp, expected_ts);

 if (buffer->nTimeStamp != expected_ts) {
                        DEBUG_PRINT_ERROR("ERROR in omx_vdec::async_message_process timestamp Check");
 }
 }
 }
 }
 }

  
 if (dynamic_buf_mode) {
        buffer->nAllocLen = sizeof(struct VideoDecoderOutputMetaData);
        buffer->nFilledLen = buffer->nFilledLen ?
 sizeof(struct VideoDecoderOutputMetaData) : 0;
 }
 if (m_cb.FillBufferDone) {
 if (buffer->nFilledLen > 0) {
 if (arbitrary_bytes)
                adjust_timestamp(buffer->nTimeStamp);
 else
                set_frame_rate(buffer->nTimeStamp);

 if (perf_flag) {
 if (!proc_frms) {
                    dec_time.stop();
                    latency = dec_time.processing_time_us() - latency;
                    DEBUG_PRINT_HIGH(">>> FBD Metrics: Latency(%.2f)mS", latency / 1e3);
                    dec_time.start();
                    fps_metrics.start();
 }
                proc_frms++;
 if (buffer->nFlags & OMX_BUFFERFLAG_EOS) {
                    OMX_U64 proc_time = 0;
                    fps_metrics.stop();
                    proc_time = fps_metrics.processing_time_us();
                    DEBUG_PRINT_HIGH(">>> FBD Metrics: proc_frms(%u) proc_time(%.2f)S fps(%.2f)",
 (unsigned int)proc_frms, (float)proc_time / 1e6,
 (float)(1e6 * proc_frms) / proc_time);
                    proc_frms = 0;
 }
 }
 }
 if (buffer->nFlags & OMX_BUFFERFLAG_EOS) {
            prev_ts = LLONG_MAX;
            rst_prev_ts = true;
 }

        pPMEMInfo = (OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO *)
 ((OMX_QCOM_PLATFORM_PRIVATE_LIST *)
             buffer->pPlatformPrivate)->entryList->entry;
        DEBUG_PRINT_LOW("Before FBD callback Accessed Pmeminfo %lu",pPMEMInfo->pmem_fd);
        OMX_BUFFERHEADERTYPE *il_buffer;
        il_buffer = client_buffers.get_il_buf_hdr(buffer);

 if (il_buffer && m_last_rendered_TS >= 0) {
 int current_framerate = (int)(drv_ctx.frame_rate.fps_numerator /drv_ctx.frame_rate.fps_denominator);
            OMX_TICKS ts_delta = (OMX_TICKS)llabs(il_buffer->nTimeStamp - m_last_rendered_TS);


 if(current_framerate <= 60 || m_last_rendered_TS == 0 ||
               il_buffer->nTimeStamp == 0 || ts_delta >= 16000 ||
               ts_delta == 0 || (il_buffer->nFlags & OMX_BUFFERFLAG_EOS)) {
               m_last_rendered_TS = il_buffer->nTimeStamp;
 } else {
               buffer->nFilledLen = 0;
 }

            DEBUG_PRINT_LOW(" -- %s Frame -- info:: fps(%d) lastRenderTime(%lld) bufferTs(%lld) ts_delta(%lld)",
                              buffer->nFilledLen? "Rendering":"Dropping",current_framerate,m_last_rendered_TS,
                              il_buffer->nTimeStamp,ts_delta);
 }

 if (il_buffer) {
            log_output_buffers(il_buffer);
 if (dynamic_buf_mode) {
 unsigned int nPortIndex = 0;
                nPortIndex = buffer-((OMX_BUFFERHEADERTYPE *)client_buffers.get_il_buf_hdr());

 if (!secure_mode) {
                    munmap(drv_ctx.ptr_outputbuffer[nPortIndex].bufferaddr,
                        drv_ctx.ptr_outputbuffer[nPortIndex].mmaped_size);
 }

                native_buffer[nPortIndex].privatehandle = NULL;
                native_buffer[nPortIndex].nativehandle = NULL;
 }
            m_cb.FillBufferDone (hComp,m_app_data,il_buffer);
 } else {
            DEBUG_PRINT_ERROR("Invalid buffer address from get_il_buf_hdr");
 return OMX_ErrorBadParameter;
 }
        DEBUG_PRINT_LOW("After Fill Buffer Done callback %lu",pPMEMInfo->pmem_fd);
 } else {
 return OMX_ErrorBadParameter;
 }

#ifdef ADAPTIVE_PLAYBACK_SUPPORTED
 if (m_smoothstreaming_mode && m_out_mem_ptr) {
        OMX_U32 buf_index = buffer - m_out_mem_ptr;
 BufferDim_t dim;
 private_handle_t *private_handle = NULL;
        dim.sliceWidth = framesize.nWidth;
        dim.sliceHeight = framesize.nHeight;
 if (native_buffer[buf_index].privatehandle)
            private_handle = native_buffer[buf_index].privatehandle;
 if (private_handle) {
            DEBUG_PRINT_LOW("set metadata: update buf-geometry with stride %d slice %d",
                dim.sliceWidth, dim.sliceHeight);
            setMetaData(private_handle, UPDATE_BUFFER_GEOMETRY, (void*)&dim);
 }
 }
#endif

 return OMX_ErrorNone;
}
