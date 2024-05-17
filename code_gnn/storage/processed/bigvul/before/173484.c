OMX_ERRORTYPE omx_vdec::push_input_hevc(OMX_HANDLETYPE hComp)
{
    OMX_U32 partial_frame = 1;
 unsigned long address,p2,id;
    OMX_BOOL isNewFrame = OMX_FALSE;
    OMX_BOOL generate_ebd = OMX_TRUE;
    OMX_ERRORTYPE rc = OMX_ErrorNone;
 if (h264_scratch.pBuffer == NULL) {
        DEBUG_PRINT_ERROR("ERROR:Hevc Scratch Buffer not allocated");
 return OMX_ErrorBadParameter;
 }

    DEBUG_PRINT_LOW("h264_scratch.nFilledLen %u has look_ahead_nal %d \
            pdest_frame nFilledLen %u nTimeStamp %lld",
 (unsigned int)h264_scratch.nFilledLen, look_ahead_nal, (unsigned int)pdest_frame->nFilledLen, pdest_frame->nTimeStamp);

 if (h264_scratch.nFilledLen && look_ahead_nal) {
        look_ahead_nal = false;
        rc = copy_buffer(pdest_frame, &h264_scratch);
 if (rc != OMX_ErrorNone) {
 return rc;
 }
 }

 if (nal_length == 0) {
 if (m_frame_parser.parse_sc_frame(psource_frame,
 &h264_scratch,&partial_frame) == -1) {
            DEBUG_PRINT_ERROR("Error In Parsing Return Error");
 return OMX_ErrorBadParameter;
 }
 } else {
        DEBUG_PRINT_LOW("Non-zero NAL length clip, hence parse with NAL size %d",nal_length);
 if (m_frame_parser.parse_h264_nallength(psource_frame,
 &h264_scratch,&partial_frame) == -1) {
            DEBUG_PRINT_ERROR("Error In Parsing NAL size, Return Error");
 return OMX_ErrorBadParameter;
 }
 }

 if (partial_frame == 0) {
 if (nal_count == 0 && h264_scratch.nFilledLen == 0) {
            DEBUG_PRINT_LOW("First NAL with Zero Length, hence Skip");
            nal_count++;
            h264_scratch.nTimeStamp = psource_frame->nTimeStamp;
            h264_scratch.nFlags = psource_frame->nFlags;
 } else {
            DEBUG_PRINT_LOW("Parsed New NAL Length = %u", (unsigned int)h264_scratch.nFilledLen);
 if (h264_scratch.nFilledLen) {
                m_hevc_utils.isNewFrame(&h264_scratch, 0, isNewFrame);
                nal_count++;
 }

 if (!isNewFrame) {
                DEBUG_PRINT_LOW("Not a new frame, copy h264_scratch nFilledLen %u \
                        nTimestamp %lld, pdest_frame nFilledLen %u nTimestamp %lld",
 (unsigned int)h264_scratch.nFilledLen, h264_scratch.nTimeStamp,
 (unsigned int)pdest_frame->nFilledLen, pdest_frame->nTimeStamp);
                rc = copy_buffer(pdest_frame, &h264_scratch);
 if (rc != OMX_ErrorNone) {
 return rc;
 }
 } else {
                look_ahead_nal = true;
 if (pdest_frame->nFilledLen == 0) {
                    look_ahead_nal = false;
                    DEBUG_PRINT_LOW("dest nation buffer empty, copy scratch buffer");
                    rc = copy_buffer(pdest_frame, &h264_scratch);
 if (rc != OMX_ErrorNone) {
 return OMX_ErrorBadParameter;
 }
 } else {
 if (psource_frame->nFilledLen || h264_scratch.nFilledLen) {
                        pdest_frame->nFlags &= ~OMX_BUFFERFLAG_EOS;
 }
                    DEBUG_PRINT_LOW("FrameDetected # %d pdest_frame nFilledLen %u \
                            nTimeStamp %lld, look_ahead_nal in h264_scratch \
                            nFilledLen %u nTimeStamp %lld",
                            frame_count++, (unsigned int)pdest_frame->nFilledLen,
                            pdest_frame->nTimeStamp, (unsigned int)h264_scratch.nFilledLen,
                            h264_scratch.nTimeStamp);
 if (empty_this_buffer_proxy(hComp, pdest_frame) != OMX_ErrorNone) {
 return OMX_ErrorBadParameter;
 }
                    pdest_frame = NULL;
 if (m_input_free_q.m_size) {
                        m_input_free_q.pop_entry(&address, &p2, &id);
                        pdest_frame = (OMX_BUFFERHEADERTYPE *) address;
                        DEBUG_PRINT_LOW("pop the next pdest_buffer %p", pdest_frame);
                        pdest_frame->nFilledLen = 0;
                        pdest_frame->nFlags = 0;
                        pdest_frame->nTimeStamp = LLONG_MAX;
 }
 }
 }
 }
 } else {
        DEBUG_PRINT_LOW("psource_frame is partial nFilledLen %u nTimeStamp %lld, \
                pdest_frame nFilledLen %u nTimeStamp %lld, h264_scratch \
                nFilledLen %u nTimeStamp %lld",
 (unsigned int)psource_frame->nFilledLen, psource_frame->nTimeStamp,
 (unsigned int)pdest_frame->nFilledLen, pdest_frame->nTimeStamp,
 (unsigned int)h264_scratch.nFilledLen, h264_scratch.nTimeStamp);

 if (h264_scratch.nAllocLen ==
                h264_scratch.nFilledLen + h264_scratch.nOffset) {
            DEBUG_PRINT_ERROR("ERROR: Frame Not found though Destination Filled");
 return OMX_ErrorStreamCorrupt;
 }
 }

 if (!psource_frame->nFilledLen) {
        DEBUG_PRINT_LOW("Buffer Consumed return source %p back to client", psource_frame);
 if (psource_frame->nFlags & OMX_BUFFERFLAG_EOS) {
 if (pdest_frame) {
                DEBUG_PRINT_LOW("EOS Reached Pass Last Buffer");
                rc = copy_buffer(pdest_frame, &h264_scratch);
 if ( rc != OMX_ErrorNone ) {
 return rc;
 }
                pdest_frame->nTimeStamp = h264_scratch.nTimeStamp;
                pdest_frame->nFlags = h264_scratch.nFlags | psource_frame->nFlags;
                DEBUG_PRINT_LOW("Push EOS frame number:%d nFilledLen =%u TimeStamp = %lld",
                        frame_count, (unsigned int)pdest_frame->nFilledLen, pdest_frame->nTimeStamp);
 if (empty_this_buffer_proxy(hComp, pdest_frame) != OMX_ErrorNone) {
 return OMX_ErrorBadParameter;
 }
                frame_count++;
                pdest_frame = NULL;
 } else {
                DEBUG_PRINT_LOW("Last frame in else dest addr %p size %u",
                        pdest_frame, (unsigned int)h264_scratch.nFilledLen);
                generate_ebd = OMX_FALSE;
 }
 }
 }

 if (generate_ebd && !psource_frame->nFilledLen) {
        m_cb.EmptyBufferDone (hComp, m_app_data, psource_frame);
        psource_frame = NULL;
 if (m_input_pending_q.m_size) {
            m_input_pending_q.pop_entry(&address, &p2, &id);
            psource_frame = (OMX_BUFFERHEADERTYPE *)address;
            DEBUG_PRINT_LOW("Next source Buffer flag %u nFilledLen %u, nTimeStamp %lld",
 (unsigned int)psource_frame->nFlags, (unsigned int)psource_frame->nFilledLen, psource_frame->nTimeStamp);
 }
 }
 return OMX_ErrorNone;
}
