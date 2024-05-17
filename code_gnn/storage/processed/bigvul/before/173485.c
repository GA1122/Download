OMX_ERRORTYPE omx_vdec::push_input_sc_codec(OMX_HANDLETYPE hComp)
{
    OMX_U32 partial_frame = 1;
    OMX_BOOL generate_ebd = OMX_TRUE;
 unsigned long address = 0, p2 = 0, id = 0;

    DEBUG_PRINT_LOW("Start Parsing the bit stream address %p TimeStamp %lld",
            psource_frame,psource_frame->nTimeStamp);
 if (m_frame_parser.parse_sc_frame(psource_frame,
                pdest_frame,&partial_frame) == -1) {
        DEBUG_PRINT_ERROR("Error In Parsing Return Error");
 return OMX_ErrorBadParameter;
 }

 if (partial_frame == 0) {
        DEBUG_PRINT_LOW("Frame size %u source %p frame count %d",
 (unsigned int)pdest_frame->nFilledLen,psource_frame,frame_count);


        DEBUG_PRINT_LOW("TimeStamp updated %lld", pdest_frame->nTimeStamp);
  
 if (frame_count == 0) {
            DEBUG_PRINT_LOW("H263/MPEG4 Codec First Frame ");

 if (codec_type_parse == CODEC_TYPE_MPEG4 ||
                    codec_type_parse == CODEC_TYPE_DIVX) {
                mp4StreamType psBits;
                psBits.data = pdest_frame->pBuffer + pdest_frame->nOffset;
                psBits.numBytes = pdest_frame->nFilledLen;
                mp4_headerparser.parseHeader(&psBits);
 }

            frame_count++;
 } else {
            pdest_frame->nFlags &= ~OMX_BUFFERFLAG_EOS;
 if (pdest_frame->nFilledLen) {
  
 if (empty_this_buffer_proxy(hComp,pdest_frame) != OMX_ErrorNone) {
 return OMX_ErrorBadParameter;
 }
                frame_count++;
                pdest_frame = NULL;

 if (m_input_free_q.m_size) {
                    m_input_free_q.pop_entry(&address,&p2,&id);
                    pdest_frame = (OMX_BUFFERHEADERTYPE *) address;
                    pdest_frame->nFilledLen = 0;
 }
 } else if (!(psource_frame->nFlags & OMX_BUFFERFLAG_EOS)) {
                DEBUG_PRINT_ERROR("Zero len buffer return back to POOL");
                m_input_free_q.insert_entry((unsigned long) pdest_frame, (unsigned)NULL,
 (unsigned)NULL);
                pdest_frame = NULL;
 }
 }
 } else {
        DEBUG_PRINT_LOW("Not a Complete Frame %u", (unsigned int)pdest_frame->nFilledLen);
  
 if (pdest_frame->nAllocLen ==
                pdest_frame->nFilledLen + pdest_frame->nOffset) {
            DEBUG_PRINT_ERROR("ERROR:Frame Not found though Destination Filled");
 return OMX_ErrorStreamCorrupt;
 }
 }

 if (psource_frame->nFilledLen == 0) {
 if (psource_frame->nFlags & OMX_BUFFERFLAG_EOS) {
 if (pdest_frame) {
                pdest_frame->nFlags |= psource_frame->nFlags;
                DEBUG_PRINT_LOW("Frame Found start Decoding Size =%u TimeStamp = %lld",
 (unsigned int)pdest_frame->nFilledLen,pdest_frame->nTimeStamp);
                DEBUG_PRINT_LOW("Found a frame size = %u number = %d",
 (unsigned int)pdest_frame->nFilledLen,frame_count++);
  
 if (empty_this_buffer_proxy(hComp,pdest_frame) != OMX_ErrorNone) {
 return OMX_ErrorBadParameter;
 }
                frame_count++;
                pdest_frame = NULL;
 } else {
                DEBUG_PRINT_LOW("Last frame in else dest addr") ;
                generate_ebd = OMX_FALSE;
 }
 }
 if (generate_ebd) {
            DEBUG_PRINT_LOW("Buffer Consumed return back to client %p",psource_frame);
            m_cb.EmptyBufferDone (hComp,m_app_data,psource_frame);
            psource_frame = NULL;

 if (m_input_pending_q.m_size) {
                DEBUG_PRINT_LOW("Pull Next source Buffer %p",psource_frame);
                m_input_pending_q.pop_entry(&address,&p2,&id);
                psource_frame = (OMX_BUFFERHEADERTYPE *) address;
                DEBUG_PRINT_LOW("Next source Buffer %p time stamp %lld",psource_frame,
                        psource_frame->nTimeStamp);
                DEBUG_PRINT_LOW("Next source Buffer flag %u length %u",
 (unsigned int)psource_frame->nFlags, (unsigned int)psource_frame->nFilledLen);
 }
 }
 }
 return OMX_ErrorNone;
}
