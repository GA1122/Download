OMX_ERRORTYPE omx_video::push_empty_eos_buffer(OMX_HANDLETYPE hComp,
        OMX_BUFFERHEADERTYPE* buffer) {
    OMX_BUFFERHEADERTYPE* opqBuf = NULL;
    OMX_ERRORTYPE retVal = OMX_ErrorNone;
 unsigned index = 0;

    DEBUG_PRINT_LOW("In push empty eos buffer");
 do {
 if (mUsesColorConversion) {
 if (pdest_frame) {
            opqBuf = pdest_frame;
            pdest_frame = NULL;
 } else if (m_opq_pmem_q.m_size) {
 unsigned long address = 0, p2, id;
            m_opq_pmem_q.pop_entry(&address,&p2,&id);
            opqBuf = (OMX_BUFFERHEADERTYPE* ) address;
 }
            index = opqBuf - m_inp_mem_ptr;
 } else {
            opqBuf = (OMX_BUFFERHEADERTYPE* ) buffer;
            index = opqBuf - meta_buffer_hdr;
 }

 if (!opqBuf || index >= m_sInPortDef.nBufferCountActual) {
            DEBUG_PRINT_ERROR("push_empty_eos_buffer: Could not find a "
 "color-conversion buffer to queue ! defer until available");
 return OMX_ErrorNone;
 }
 struct pmem Input_pmem_info;
 Input_pmem_info.buffer = opqBuf;
 Input_pmem_info.fd = m_pInput_pmem[index].fd;
 Input_pmem_info.offset = 0;
 Input_pmem_info.size = m_pInput_pmem[index].size;

 if (dev_use_buf(&Input_pmem_info, PORT_INDEX_IN, 0) != true) {
            DEBUG_PRINT_ERROR("ERROR: in dev_use_buf for empty eos buffer");
            retVal = OMX_ErrorBadParameter;
 break;
 }

        OMX_BUFFERHEADERTYPE emptyEosBufHdr;
        memcpy(&emptyEosBufHdr, opqBuf, sizeof(OMX_BUFFERHEADERTYPE));
        emptyEosBufHdr.nFilledLen = 0;
        emptyEosBufHdr.nTimeStamp = buffer->nTimeStamp;
        emptyEosBufHdr.nFlags = buffer->nFlags;
        emptyEosBufHdr.pBuffer = NULL;
 if (!mUsesColorConversion)
            emptyEosBufHdr.nAllocLen = m_sInPortDef.nBufferSize;
 if (dev_empty_buf(&emptyEosBufHdr, 0, index, m_pInput_pmem[index].fd) != true) {
            DEBUG_PRINT_ERROR("ERROR: in dev_empty_buf for empty eos buffer");
            dev_free_buf(&Input_pmem_info, PORT_INDEX_IN);
            retVal = OMX_ErrorBadParameter;
 break;
 }
        mEmptyEosBuffer = opqBuf;
 } while(false);

    m_pCallbacks.EmptyBufferDone(hComp, m_app_data, buffer);
 --pending_input_buffers;
 return retVal;
}
