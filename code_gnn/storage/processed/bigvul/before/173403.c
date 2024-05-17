OMX_ERRORTYPE  omx_vdec::allocate_output_buffer(
        OMX_IN OMX_HANDLETYPE            hComp,
        OMX_INOUT OMX_BUFFERHEADERTYPE** bufferHdr,
        OMX_IN OMX_U32                   port,
        OMX_IN OMX_PTR                   appData,
        OMX_IN OMX_U32                   bytes)
{
 (void)hComp;
 (void)port;
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE       *bufHdr= NULL;  
 unsigned                         i= 0;  
 struct vdec_setbuffer_cmd setbuffers;
 int extra_idx = 0;
#ifdef USE_ION
 int ion_device_fd =-1;
 struct ion_allocation_data ion_alloc_data;
 struct ion_fd_data fd_ion_data;
#endif
 if (!m_out_mem_ptr) {
        DEBUG_PRINT_HIGH("Allocate o/p buffer Header: Cnt(%d) Sz(%u)",
                drv_ctx.op_buf.actualcount,
 (unsigned int)drv_ctx.op_buf.buffer_size);
 int nBufHdrSize        = 0;
 int nPlatformEntrySize = 0;
 int nPlatformListSize  = 0;
 int nPMEMInfoSize = 0;
 int pmem_fd = -1;
 unsigned char *pmem_baseaddress = NULL;

        OMX_QCOM_PLATFORM_PRIVATE_LIST      *pPlatformList;
        OMX_QCOM_PLATFORM_PRIVATE_ENTRY     *pPlatformEntry;
        OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO *pPMEMInfo;

        DEBUG_PRINT_LOW("Allocating First Output Buffer(%d)",
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
        DEBUG_PRINT_LOW("PE %d OutputBuffer Count %d",nPlatformEntrySize,
                drv_ctx.op_buf.actualcount);
#ifdef USE_ION
 int cache_flag = client_buffers.is_color_conversion_enabled() ? 0 : ION_FLAG_CACHED;
        ion_device_fd = alloc_map_ion_memory(
                drv_ctx.op_buf.buffer_size * drv_ctx.op_buf.actualcount,
                secure_scaling_to_non_secure_opb ? SZ_4K : drv_ctx.op_buf.alignment,
 &ion_alloc_data, &fd_ion_data,
 (secure_mode && !secure_scaling_to_non_secure_opb) ? ION_SECURE : cache_flag);
 if (ion_device_fd < 0) {
 return OMX_ErrorInsufficientResources;
 }
        pmem_fd = fd_ion_data.fd;
#else
        pmem_fd = open (MEM_DEVICE,O_RDWR);

 if (pmem_fd < 0) {
            DEBUG_PRINT_ERROR("ERROR:pmem fd for output buffer %d",
                    drv_ctx.op_buf.buffer_size);
 return OMX_ErrorInsufficientResources;
 }

 if (pmem_fd == 0) {
            pmem_fd = open (MEM_DEVICE,O_RDWR);

 if (pmem_fd < 0) {
                DEBUG_PRINT_ERROR("ERROR:pmem fd for output buffer %d",
                        drv_ctx.op_buf.buffer_size);
 return OMX_ErrorInsufficientResources;
 }
 }

 if (!align_pmem_buffers(pmem_fd, drv_ctx.op_buf.buffer_size *
                    drv_ctx.op_buf.actualcount,
                    drv_ctx.op_buf.alignment)) {
            DEBUG_PRINT_ERROR("align_pmem_buffers() failed");
            close(pmem_fd);
 return OMX_ErrorInsufficientResources;
 }
#endif
 if (!secure_mode) {
            pmem_baseaddress = (unsigned char *)mmap(NULL,
 (drv_ctx.op_buf.buffer_size *
                     drv_ctx.op_buf.actualcount),
                    PROT_READ|PROT_WRITE,MAP_SHARED,pmem_fd,0);
 if (pmem_baseaddress == MAP_FAILED) {
                DEBUG_PRINT_ERROR("MMAP failed for Size %u",
 (unsigned int)drv_ctx.op_buf.buffer_size);
                close(pmem_fd);
#ifdef USE_ION
                free_ion_memory(&drv_ctx.op_buf_ion_info[i]);
#endif
 return OMX_ErrorInsufficientResources;
 }
 }
        m_out_mem_ptr = (OMX_BUFFERHEADERTYPE  *)calloc(nBufHdrSize,1);
 char *pPtr=NULL;
        pPtr = (char*) calloc(nPlatformListSize + nPlatformEntrySize +
                nPMEMInfoSize,1);
        drv_ctx.ptr_outputbuffer = (struct vdec_bufferpayload *)\
                       calloc (sizeof(struct vdec_bufferpayload),
                               drv_ctx.op_buf.actualcount);
        drv_ctx.ptr_respbuffer = (struct vdec_output_frameinfo  *)\
                     calloc (sizeof (struct vdec_output_frameinfo),
                             drv_ctx.op_buf.actualcount);
 if (!drv_ctx.ptr_outputbuffer || !drv_ctx.ptr_respbuffer) {
            DEBUG_PRINT_ERROR("Failed to alloc drv_ctx.ptr_outputbuffer or drv_ctx.ptr_respbuffer ");
 return OMX_ErrorInsufficientResources;
 }

#ifdef USE_ION
        drv_ctx.op_buf_ion_info = (struct vdec_ion *)\
                      calloc (sizeof(struct vdec_ion),
                              drv_ctx.op_buf.actualcount);
 if (!drv_ctx.op_buf_ion_info) {
            DEBUG_PRINT_ERROR("Failed to alloc drv_ctx.op_buf_ion_info");
 return OMX_ErrorInsufficientResources;
 }
#endif

 if (m_out_mem_ptr && pPtr && drv_ctx.ptr_outputbuffer
 && drv_ctx.ptr_respbuffer) {
            drv_ctx.ptr_outputbuffer[0].mmaped_size =
 (drv_ctx.op_buf.buffer_size *
                 drv_ctx.op_buf.actualcount);
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

            DEBUG_PRINT_LOW("bHdr %p OutMem %p PE %p",bufHdr, m_out_mem_ptr,pPlatformEntry);
            DEBUG_PRINT_LOW(" Pmem Info = %p",pPMEMInfo);
 for (i=0; i < drv_ctx.op_buf.actualcount ; i++) {
                bufHdr->nSize              = sizeof(OMX_BUFFERHEADERTYPE);
                bufHdr->nVersion.nVersion  = OMX_SPEC_VERSION;
                bufHdr->nAllocLen          = bytes;
                bufHdr->nFilledLen         = 0;
                bufHdr->pAppPrivate        = appData;
                bufHdr->nOutputPortIndex   = OMX_CORE_OUTPUT_PORT_INDEX;
                pPlatformEntry->type       = OMX_QCOM_PLATFORM_PRIVATE_PMEM;
                pPlatformEntry->entry      = pPMEMInfo;
                pPlatformList->nEntries    = 1;
                pPlatformList->entryList   = pPlatformEntry;
                bufHdr->pBuffer            = NULL;
                bufHdr->nOffset            = 0;

                pPMEMInfo->offset          =  drv_ctx.op_buf.buffer_size*i;
                pPMEMInfo->pmem_fd = 0;
                bufHdr->pPlatformPrivate = pPlatformList;

                drv_ctx.ptr_outputbuffer[i].pmem_fd = pmem_fd;
                m_pmem_info[i].pmem_fd = pmem_fd;
#ifdef USE_ION
                drv_ctx.op_buf_ion_info[i].ion_device_fd = ion_device_fd;
                drv_ctx.op_buf_ion_info[i].ion_alloc_data = ion_alloc_data;
                drv_ctx.op_buf_ion_info[i].fd_ion_data = fd_ion_data;
#endif

  
                bufHdr->pOutputPortPrivate = &drv_ctx.ptr_respbuffer[i];
                drv_ctx.ptr_respbuffer[i].client_data = (void *)\
 &drv_ctx.ptr_outputbuffer[i];
                drv_ctx.ptr_outputbuffer[i].offset = drv_ctx.op_buf.buffer_size*i;
                drv_ctx.ptr_outputbuffer[i].bufferaddr =
                    pmem_baseaddress + (drv_ctx.op_buf.buffer_size*i);
                m_pmem_info[i].size = drv_ctx.ptr_outputbuffer[i].buffer_len;
                m_pmem_info[i].mapped_size = drv_ctx.ptr_outputbuffer[i].mmaped_size;
                m_pmem_info[i].buffer = drv_ctx.ptr_outputbuffer[i].bufferaddr;

                DEBUG_PRINT_LOW("pmem_fd = %d offset = %u address = %p",
                        pmem_fd, (unsigned int)drv_ctx.ptr_outputbuffer[i].offset,
                        drv_ctx.ptr_outputbuffer[i].bufferaddr);
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
 if (eRet == OMX_ErrorNone)
            eRet = allocate_extradata();
 }

 for (i=0; i< drv_ctx.op_buf.actualcount; i++) {
 if (BITMASK_ABSENT(&m_out_bm_count,i)) {
            DEBUG_PRINT_LOW("Found a Free Output Buffer %d",i);
 break;
 }
 }

 if (eRet == OMX_ErrorNone) {
 if (i < drv_ctx.op_buf.actualcount) {
 struct v4l2_buffer buf;
 struct v4l2_plane plane[VIDEO_MAX_PLANES];
 int rc;
            m_pmem_info[i].offset = drv_ctx.ptr_outputbuffer[i].offset;

            drv_ctx.ptr_outputbuffer[i].buffer_len =
                drv_ctx.op_buf.buffer_size;

 *bufferHdr = (m_out_mem_ptr + i );
 if (secure_mode) {
#ifdef USE_ION
                drv_ctx.ptr_outputbuffer[i].bufferaddr =
 (OMX_U8 *)(intptr_t)drv_ctx.op_buf_ion_info[i].fd_ion_data.fd;
#else
                drv_ctx.ptr_outputbuffer[i].bufferaddr = *bufferHdr;
#endif
 }
            drv_ctx.ptr_outputbuffer[i].mmaped_size = drv_ctx.op_buf.buffer_size;

            buf.index = i;
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
            buf.memory = V4L2_MEMORY_USERPTR;
            plane[0].length = drv_ctx.op_buf.buffer_size;
            plane[0].m.userptr = (unsigned long)drv_ctx.ptr_outputbuffer[i].bufferaddr -
 (unsigned long)drv_ctx.ptr_outputbuffer[i].offset;
#ifdef USE_ION
            plane[0].reserved[0] = drv_ctx.op_buf_ion_info[i].fd_ion_data.fd;
#endif
            plane[0].reserved[1] = drv_ctx.ptr_outputbuffer[i].offset;
            plane[0].data_offset = 0;
            extra_idx = EXTRADATA_IDX(drv_ctx.num_planes);
 if (extra_idx && (extra_idx < VIDEO_MAX_PLANES)) {
                plane[extra_idx].length = drv_ctx.extradata_info.buffer_size;
                plane[extra_idx].m.userptr = (long unsigned int) (drv_ctx.extradata_info.uaddr + i * drv_ctx.extradata_info.buffer_size);
#ifdef USE_ION
                plane[extra_idx].reserved[0] = drv_ctx.extradata_info.ion.fd_ion_data.fd;
#endif
                plane[extra_idx].reserved[1] = i * drv_ctx.extradata_info.buffer_size;
                plane[extra_idx].data_offset = 0;
 } else if (extra_idx >= VIDEO_MAX_PLANES) {
                DEBUG_PRINT_ERROR("Extradata index higher than allowed: %d", extra_idx);
 return OMX_ErrorBadParameter;
 }
            buf.m.planes = plane;
            buf.length = drv_ctx.num_planes;
            DEBUG_PRINT_LOW("Set the Output Buffer Idx: %d Addr: %p", i, drv_ctx.ptr_outputbuffer[i].bufferaddr);
            rc = ioctl(drv_ctx.video_driver_fd, VIDIOC_PREPARE_BUF, &buf);
 if (rc) {
  
 return OMX_ErrorInsufficientResources;
 }

 if (i == (drv_ctx.op_buf.actualcount -1 ) && !streaming[CAPTURE_PORT]) {
 enum v4l2_buf_type buf_type;
                buf_type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
                rc=ioctl(drv_ctx.video_driver_fd, VIDIOC_STREAMON,&buf_type);
 if (rc) {
 return OMX_ErrorInsufficientResources;
 } else {
                    streaming[CAPTURE_PORT] = true;
                    DEBUG_PRINT_LOW("STREAMON Successful");
 }
 }

 (*bufferHdr)->pBuffer = (OMX_U8*)drv_ctx.ptr_outputbuffer[i].bufferaddr;
 (*bufferHdr)->pAppPrivate = appData;
            BITMASK_SET(&m_out_bm_count,i);
 } else {
            DEBUG_PRINT_ERROR("All the Output Buffers have been Allocated ; Returning Insufficient");
            eRet = OMX_ErrorInsufficientResources;
 }
 }

 return eRet;
}
