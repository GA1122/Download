OMX_ERRORTYPE omx_video::allocate_input_meta_buffer(
        OMX_HANDLETYPE       hComp,
        OMX_BUFFERHEADERTYPE **bufferHdr,
        OMX_PTR              appData,
        OMX_U32              bytes)
{
 unsigned index = 0;
 if (!bufferHdr || bytes < sizeof(encoder_media_buffer_type)) {
        DEBUG_PRINT_ERROR("wrong params allocate_input_meta_buffer Hdr %p len %u",
                bufferHdr, (unsigned int)bytes);
 return OMX_ErrorBadParameter;
 }

 if (!m_inp_mem_ptr && !mUseProxyColorFormat) {
        m_inp_mem_ptr = meta_buffer_hdr;
        DEBUG_PRINT_LOW("use meta_buffer_hdr (%p) as m_inp_mem_ptr = %p",
                meta_buffer_hdr, m_inp_mem_ptr);
 }
 for (index = 0; ((index < m_sInPortDef.nBufferCountActual) &&
                meta_buffer_hdr[index].pBuffer); index++);
 if (index == m_sInPortDef.nBufferCountActual) {
        DEBUG_PRINT_ERROR("All buffers are allocated input_meta_buffer");
 return OMX_ErrorBadParameter;
 }
 if (mUseProxyColorFormat) {
 if (opaque_buffer_hdr[index]) {
            DEBUG_PRINT_ERROR("All buffers are allocated opaque_buffer_hdr");
 return OMX_ErrorBadParameter;
 }
 if (allocate_input_buffer(hComp,&opaque_buffer_hdr[index],
                    PORT_INDEX_IN,appData,m_sInPortDef.nBufferSize) != OMX_ErrorNone) {
            DEBUG_PRINT_ERROR("All buffers are allocated opaque_buffer_hdr");
 return OMX_ErrorBadParameter;
 }
 }
    BITMASK_SET(&m_inp_bm_count,index);
 *bufferHdr = &meta_buffer_hdr[index];
    memset(&meta_buffer_hdr[index], 0, sizeof(meta_buffer_hdr[index]));
    meta_buffer_hdr[index].nSize = sizeof(meta_buffer_hdr[index]);
    meta_buffer_hdr[index].nAllocLen = bytes;
    meta_buffer_hdr[index].nVersion.nVersion = OMX_SPEC_VERSION;
    meta_buffer_hdr[index].nInputPortIndex = PORT_INDEX_IN;
    meta_buffer_hdr[index].pBuffer = (OMX_U8*)&meta_buffers[index];
    meta_buffer_hdr[index].pAppPrivate = appData;
 if (mUseProxyColorFormat) {
        m_opq_pmem_q.insert_entry((unsigned long)opaque_buffer_hdr[index],0,0);
        DEBUG_PRINT_HIGH("opaque_buffer_hdr insert %p", opaque_buffer_hdr[index]);
 }
 return OMX_ErrorNone;
}
