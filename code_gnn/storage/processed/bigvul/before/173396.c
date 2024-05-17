OMX_ERRORTYPE omx_vdec::allocate_color_convert_buf::allocate_buffers_color_convert(OMX_HANDLETYPE hComp,
        OMX_BUFFERHEADERTYPE **bufferHdr,OMX_U32 port,OMX_PTR appData,OMX_U32 bytes)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 if (!enabled) {
        eRet = omx->allocate_output_buffer(hComp,bufferHdr,port,appData,bytes);
 return eRet;
 }
 if (enabled && omx->is_component_secure()) {
        DEBUG_PRINT_ERROR("Notin color convert mode secure_mode %d",
                omx->is_component_secure());
 return OMX_ErrorUnsupportedSetting;
 }
 if (!bufferHdr || bytes > buffer_size_req) {
        DEBUG_PRINT_ERROR("Invalid params allocate_buffers_color_convert %p", bufferHdr);
        DEBUG_PRINT_ERROR("color_convert buffer_size_req %u bytes %u",
 (unsigned int)buffer_size_req, (unsigned int)bytes);
 return OMX_ErrorBadParameter;
 }
 if (allocated_count >= omx->drv_ctx.op_buf.actualcount) {
        DEBUG_PRINT_ERROR("Actual count err in allocate_buffers_color_convert");
 return OMX_ErrorInsufficientResources;
 }
    OMX_BUFFERHEADERTYPE *temp_bufferHdr = NULL;
    eRet = omx->allocate_output_buffer(hComp,&temp_bufferHdr,
            port,appData,omx->drv_ctx.op_buf.buffer_size);
 if (eRet != OMX_ErrorNone || !temp_bufferHdr) {
        DEBUG_PRINT_ERROR("Buffer allocation failed color_convert");
 return eRet;
 }
 if ((temp_bufferHdr - omx->m_out_mem_ptr) >=
 (int)omx->drv_ctx.op_buf.actualcount) {
        DEBUG_PRINT_ERROR("Invalid header index %ld",
 (long int)(temp_bufferHdr - omx->m_out_mem_ptr));
 return OMX_ErrorUndefined;
 }
 unsigned int i = allocated_count;
#ifdef USE_ION
    op_buf_ion_info[i].ion_device_fd = omx->alloc_map_ion_memory(
            buffer_size_req,buffer_alignment_req,
 &op_buf_ion_info[i].ion_alloc_data,&op_buf_ion_info[i].fd_ion_data,
            ION_FLAG_CACHED);
    pmem_fd[i] = op_buf_ion_info[i].fd_ion_data.fd;
 if (op_buf_ion_info[i].ion_device_fd < 0) {
        DEBUG_PRINT_ERROR("alloc_map_ion failed in color_convert");
 return OMX_ErrorInsufficientResources;
 }
    pmem_baseaddress[i] = (unsigned char *)mmap(NULL,buffer_size_req,
            PROT_READ|PROT_WRITE,MAP_SHARED,pmem_fd[i],0);

 if (pmem_baseaddress[i] == MAP_FAILED) {
        DEBUG_PRINT_ERROR("MMAP failed for Size %d",buffer_size_req);
        close(pmem_fd[i]);
        omx->free_ion_memory(&op_buf_ion_info[i]);
 return OMX_ErrorInsufficientResources;
 }
    m_heap_ptr[i].video_heap_ptr = new VideoHeap (
            op_buf_ion_info[i].ion_device_fd,buffer_size_req,
            pmem_baseaddress[i],op_buf_ion_info[i].ion_alloc_data.handle,pmem_fd[i]);
#endif
    m_pmem_info_client[i].pmem_fd = (unsigned long)m_heap_ptr[i].video_heap_ptr.get();
    m_pmem_info_client[i].offset = 0;
    m_platform_entry_client[i].entry = (void *)&m_pmem_info_client[i];
    m_platform_entry_client[i].type = OMX_QCOM_PLATFORM_PRIVATE_PMEM;
    m_platform_list_client[i].nEntries = 1;
    m_platform_list_client[i].entryList = &m_platform_entry_client[i];
    m_out_mem_ptr_client[i].pOutputPortPrivate = NULL;
    m_out_mem_ptr_client[i].nAllocLen = buffer_size_req;
    m_out_mem_ptr_client[i].nFilledLen = 0;
    m_out_mem_ptr_client[i].nFlags = 0;
    m_out_mem_ptr_client[i].nOutputPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    m_out_mem_ptr_client[i].nSize = sizeof(OMX_BUFFERHEADERTYPE);
    m_out_mem_ptr_client[i].nVersion.nVersion = OMX_SPEC_VERSION;
    m_out_mem_ptr_client[i].pPlatformPrivate = &m_platform_list_client[i];
    m_out_mem_ptr_client[i].pBuffer = pmem_baseaddress[i];
    m_out_mem_ptr_client[i].pAppPrivate = appData;
 *bufferHdr = &m_out_mem_ptr_client[i];
    DEBUG_PRINT_HIGH("IL client buffer header %p", *bufferHdr);
    allocated_count++;
 return eRet;
}
