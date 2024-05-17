OMX_ERRORTYPE omx_video::convert_queue_buffer(OMX_HANDLETYPE hComp,
 struct pmem &Input_pmem_info,unsigned long &index)
{

 unsigned char *uva;
    OMX_ERRORTYPE ret = OMX_ErrorNone;
 unsigned long address = 0,p2,id;

    DEBUG_PRINT_LOW("In Convert and queue Meta Buffer");
 if (!psource_frame || !pdest_frame) {
        DEBUG_PRINT_ERROR("convert_queue_buffer invalid params");
 return OMX_ErrorBadParameter;
 }
 if (secure_session) {
        DEBUG_PRINT_ERROR("cannot convert buffer during secure session");
 return OMX_ErrorInvalidState;
 }

 if (!psource_frame->nFilledLen) {
 if(psource_frame->nFlags & OMX_BUFFERFLAG_EOS) {
            pdest_frame->nFilledLen = psource_frame->nFilledLen;
            pdest_frame->nTimeStamp = psource_frame->nTimeStamp;
            pdest_frame->nFlags = psource_frame->nFlags;
            DEBUG_PRINT_HIGH("Skipping color conversion for empty EOS Buffer "
 "header=%p filled-len=%u", pdest_frame, (unsigned int)pdest_frame->nFilledLen);
 } else {
            pdest_frame->nOffset = 0;
            pdest_frame->nFilledLen = 0;
            pdest_frame->nTimeStamp = psource_frame->nTimeStamp;
            pdest_frame->nFlags = psource_frame->nFlags;
            DEBUG_PRINT_LOW("Buffer header %p Filled len size %u",
                    pdest_frame, (unsigned int)pdest_frame->nFilledLen);
 }
 } else {
        uva = (unsigned char *)mmap(NULL, Input_pmem_info.size,
                PROT_READ|PROT_WRITE,
                MAP_SHARED,Input_pmem_info.fd,0);
 if (uva == MAP_FAILED) {
            ret = OMX_ErrorBadParameter;
 } else {
 if (!c2d_conv.convert(Input_pmem_info.fd, uva, uva,
                        m_pInput_pmem[index].fd, pdest_frame->pBuffer, pdest_frame->pBuffer)) {
                DEBUG_PRINT_ERROR("Color Conversion failed");
                ret = OMX_ErrorBadParameter;
 } else {
 unsigned int buf_size = 0;
 if (!c2d_conv.get_buffer_size(C2D_OUTPUT,buf_size))
                    ret = OMX_ErrorBadParameter;
 else {
                    pdest_frame->nOffset = 0;
                    pdest_frame->nFilledLen = buf_size;
                    pdest_frame->nTimeStamp = psource_frame->nTimeStamp;
                    pdest_frame->nFlags = psource_frame->nFlags;
                    DEBUG_PRINT_LOW("Buffer header %p Filled len size %u",
                            pdest_frame, (unsigned int)pdest_frame->nFilledLen);
 }
 }
            munmap(uva,Input_pmem_info.size);
 }
 }
 if (dev_use_buf(&m_pInput_pmem[index],PORT_INDEX_IN,0) != true) {
        DEBUG_PRINT_ERROR("ERROR: in dev_use_buf");
        post_event ((unsigned long)pdest_frame,0,OMX_COMPONENT_GENERATE_EBD);
        ret = OMX_ErrorBadParameter;
 }
 if (ret == OMX_ErrorNone)
        ret = empty_this_buffer_proxy(hComp,pdest_frame);
 if (ret == OMX_ErrorNone) {
        m_pCallbacks.EmptyBufferDone(hComp ,m_app_data, psource_frame);
        psource_frame = NULL;
        pdest_frame = NULL;
 if (!psource_frame && m_opq_meta_q.m_size) {
            m_opq_meta_q.pop_entry(&address,&p2,&id);
            psource_frame = (OMX_BUFFERHEADERTYPE* ) address;
 }
 if (!pdest_frame && m_opq_pmem_q.m_size) {
            m_opq_pmem_q.pop_entry(&address,&p2,&id);
            pdest_frame = (OMX_BUFFERHEADERTYPE* ) address;
            DEBUG_PRINT_LOW("pdest_frame pop address is %p",pdest_frame);
 }
 } else {
        psource_frame = NULL;
 }
 return ret;
}
