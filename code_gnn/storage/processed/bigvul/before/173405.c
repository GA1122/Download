OMX_ERRORTYPE omx_vdec::allocate_output_headers()
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE *bufHdr = NULL;
 unsigned i= 0;

 if (!m_out_mem_ptr) {
        DEBUG_PRINT_HIGH("Use o/p buffer case - Header List allocation");
 int nBufHdrSize        = 0;
 int nPlatformEntrySize = 0;
 int nPlatformListSize  = 0;
 int nPMEMInfoSize = 0;
        OMX_QCOM_PLATFORM_PRIVATE_LIST      *pPlatformList;
        OMX_QCOM_PLATFORM_PRIVATE_ENTRY     *pPlatformEntry;
        OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO *pPMEMInfo;

        DEBUG_PRINT_LOW("Setting First Output Buffer(%d)",
                drv_ctx.op_buf.actualcount);
        nBufHdrSize        = drv_ctx.op_buf.actualcount *
 sizeof(OMX_BUFFERHEADERTYPE);

        nPMEMInfoSize      = drv_ctx.op_buf.actualcount *
 sizeof(OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO);
        nPlatformListSize  = drv_ctx.op_buf.actualcount *
 sizeof(OMX_QCOM_PLATFORM_PRIVATE_LIST);
        nPlatformEntrySize = drv_ctx.op_buf.actualcount *
 sizeof(OMX_QCOM_PLATFORM_PRIVATE_ENTRY);

        DEBUG_PRINT_LOW("TotalBufHdr %d BufHdrSize %u PMEM %d PL %d",nBufHdrSize,
 (unsigned int)sizeof(OMX_BUFFERHEADERTYPE),
                nPMEMInfoSize,
                nPlatformListSize);
        DEBUG_PRINT_LOW("PE %d bmSize % " PRId64 , nPlatformEntrySize,
                m_out_bm_count);
        m_out_mem_ptr = (OMX_BUFFERHEADERTYPE  *)calloc(nBufHdrSize,1);
 char *pPtr=NULL;
        pPtr = (char*) calloc(nPlatformListSize + nPlatformEntrySize +
                nPMEMInfoSize,1);
        drv_ctx.ptr_outputbuffer = (struct vdec_bufferpayload *) \
                       calloc (sizeof(struct vdec_bufferpayload),
                               drv_ctx.op_buf.actualcount);
        drv_ctx.ptr_respbuffer = (struct vdec_output_frameinfo  *)\
                     calloc (sizeof (struct vdec_output_frameinfo),
                             drv_ctx.op_buf.actualcount);
 if (!drv_ctx.ptr_outputbuffer || !drv_ctx.ptr_respbuffer) {
            DEBUG_PRINT_ERROR("Failed to alloc drv_ctx.ptr_outputbuffer or drv_ctx.ptr_respbuffer");
 return OMX_ErrorInsufficientResources;
 }

#ifdef USE_ION
        drv_ctx.op_buf_ion_info = (struct vdec_ion * ) \
                      calloc (sizeof(struct vdec_ion),drv_ctx.op_buf.actualcount);
 if (!drv_ctx.op_buf_ion_info) {
            DEBUG_PRINT_ERROR("Failed to alloc drv_ctx.op_buf_ion_info");
 return OMX_ErrorInsufficientResources;
 }
#endif
 if (dynamic_buf_mode) {
            out_dynamic_list = (struct dynamic_buf_list *) \
                calloc (sizeof(struct dynamic_buf_list), drv_ctx.op_buf.actualcount);
 }

 if (m_out_mem_ptr && pPtr && drv_ctx.ptr_outputbuffer
 && drv_ctx.ptr_respbuffer) {
            bufHdr          =  m_out_mem_ptr;
            m_platform_list = (OMX_QCOM_PLATFORM_PRIVATE_LIST *)(pPtr);
            m_platform_entry= (OMX_QCOM_PLATFORM_PRIVATE_ENTRY *)
 (((char *) m_platform_list) + nPlatformListSize);
            m_pmem_info     = (OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO *)
 (((char *) m_platform_entry) + nPlatformEntrySize);
            pPlatformList   = m_platform_list;
            pPlatformEntry  = m_platform_entry;
            pPMEMInfo       = m_pmem_info;

            DEBUG_PRINT_LOW("Memory Allocation Succeeded for OUT port%p",m_out_mem_ptr);

            DEBUG_PRINT_LOW("bHdr %p OutMem %p PE %p",bufHdr,
                    m_out_mem_ptr,pPlatformEntry);
            DEBUG_PRINT_LOW(" Pmem Info = %p",pPMEMInfo);
 for (i=0; i < drv_ctx.op_buf.actualcount ; i++) {
                bufHdr->nSize              = sizeof(OMX_BUFFERHEADERTYPE);
                bufHdr->nVersion.nVersion  = OMX_SPEC_VERSION;
                bufHdr->nAllocLen          = 0;
                bufHdr->nFilledLen         = 0;
                bufHdr->pAppPrivate        = NULL;
                bufHdr->nOutputPortIndex   = OMX_CORE_OUTPUT_PORT_INDEX;
                pPlatformEntry->type       = OMX_QCOM_PLATFORM_PRIVATE_PMEM;
                pPlatformEntry->entry      = pPMEMInfo;
                pPlatformList->nEntries    = 1;
                pPlatformList->entryList   = pPlatformEntry;
                bufHdr->pBuffer            = NULL;
                pPMEMInfo->offset          = 0;
                pPMEMInfo->pmem_fd = 0;
                bufHdr->pPlatformPrivate = pPlatformList;
                drv_ctx.ptr_outputbuffer[i].pmem_fd = -1;
#ifdef USE_ION
                drv_ctx.op_buf_ion_info[i].ion_device_fd =-1;
#endif
  
                bufHdr->pOutputPortPrivate = &drv_ctx.ptr_respbuffer[i];
                drv_ctx.ptr_respbuffer[i].client_data = (void *) \
 &drv_ctx.ptr_outputbuffer[i];
                bufHdr++;
                pPMEMInfo++;
                pPlatformEntry++;
                pPlatformList++;
 }
 } else {
            DEBUG_PRINT_ERROR("Output buf mem alloc failed[0x%p][0x%p]",\
                    m_out_mem_ptr, pPtr);
 if (m_out_mem_ptr) {
                free(m_out_mem_ptr);
                m_out_mem_ptr = NULL;
 }
 if (pPtr) {
                free(pPtr);
                pPtr = NULL;
 }
 if (drv_ctx.ptr_outputbuffer) {
                free(drv_ctx.ptr_outputbuffer);
                drv_ctx.ptr_outputbuffer = NULL;
 }
 if (drv_ctx.ptr_respbuffer) {
                free(drv_ctx.ptr_respbuffer);
                drv_ctx.ptr_respbuffer = NULL;
 }
#ifdef USE_ION
 if (drv_ctx.op_buf_ion_info) {
                DEBUG_PRINT_LOW("Free o/p ion context");
                free(drv_ctx.op_buf_ion_info);
                drv_ctx.op_buf_ion_info = NULL;
 }
#endif
            eRet =  OMX_ErrorInsufficientResources;
 }
 } else {
        eRet =  OMX_ErrorInsufficientResources;
 }
 return eRet;
}
