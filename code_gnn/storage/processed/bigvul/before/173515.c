OMX_ERRORTYPE  omx_vdec::use_output_buffer(
        OMX_IN OMX_HANDLETYPE            hComp,
        OMX_INOUT OMX_BUFFERHEADERTYPE** bufferHdr,
        OMX_IN OMX_U32                   port,
        OMX_IN OMX_PTR                   appData,
        OMX_IN OMX_U32                   bytes,
        OMX_IN OMX_U8*                   buffer)
{
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE       *bufHdr= NULL;  
 unsigned                         i= 0;  
 struct vdec_setbuffer_cmd setbuffers;
    OMX_PTR privateAppData = NULL;
 private_handle_t *handle = NULL;
    OMX_U8 *buff = buffer;
 struct v4l2_buffer buf;
 struct v4l2_plane plane[VIDEO_MAX_PLANES];
 int extra_idx = 0;
 (void) hComp;
 (void) port;

 if (!m_out_mem_ptr) {
        DEBUG_PRINT_HIGH("Use_op_buf:Allocating output headers");
        eRet = allocate_output_headers();
 if (eRet == OMX_ErrorNone)
            eRet = allocate_extradata();
 }

 if (eRet == OMX_ErrorNone) {
 for (i=0; i< drv_ctx.op_buf.actualcount; i++) {
 if (BITMASK_ABSENT(&m_out_bm_count,i)) {
 break;
 }
 }
 }

 if (i >= drv_ctx.op_buf.actualcount) {
        DEBUG_PRINT_ERROR("Already using %d o/p buffers", drv_ctx.op_buf.actualcount);
        eRet = OMX_ErrorInsufficientResources;
 }

 if (eRet != OMX_ErrorNone)
 return eRet;

 if (dynamic_buf_mode) {
 *bufferHdr = (m_out_mem_ptr + i );
 (*bufferHdr)->pBuffer = NULL;
 if (i == (drv_ctx.op_buf.actualcount -1) && !streaming[CAPTURE_PORT]) {
 enum v4l2_buf_type buf_type;
 int rr = 0;
            buf_type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
 if (rr = ioctl(drv_ctx.video_driver_fd, VIDIOC_STREAMON,&buf_type)) {
                DEBUG_PRINT_ERROR(" STREAMON FAILED : %d",rr);
 return OMX_ErrorInsufficientResources;
 } else {
                streaming[CAPTURE_PORT] = true;
                DEBUG_PRINT_LOW("STREAMON Successful");
 }
 }
        BITMASK_SET(&m_out_bm_count,i);
 (*bufferHdr)->pAppPrivate = appData;
 (*bufferHdr)->pBuffer = buffer;
 (*bufferHdr)->nAllocLen = sizeof(struct VideoDecoderOutputMetaData);
 return eRet;
 }

 if (eRet == OMX_ErrorNone) {
#if defined(_ANDROID_HONEYCOMB_) || defined(_ANDROID_ICS_)
 if (m_enable_android_native_buffers) {
 if (m_use_android_native_buffers) {
 UseAndroidNativeBufferParams *params = (UseAndroidNativeBufferParams *)appData;
                sp<android_native_buffer_t> nBuf = params->nativeBuffer;
                handle = (private_handle_t *)nBuf->handle;
                privateAppData = params->pAppPrivate;
 } else {
                handle = (private_handle_t *)buff;
                privateAppData = appData;
 }
 if (!handle) {
                DEBUG_PRINT_ERROR("handle is invalid");
 return OMX_ErrorBadParameter;
 }

 if ((OMX_U32)handle->size < drv_ctx.op_buf.buffer_size) {
 if (secure_mode && secure_scaling_to_non_secure_opb) {
                    DEBUG_PRINT_HIGH("Buffer size expected %u, got %u, but it's ok since we will never map it",
 (unsigned int)drv_ctx.op_buf.buffer_size, (unsigned int)handle->size);
 } else {
                DEBUG_PRINT_ERROR("Insufficient sized buffer given for playback,"
 " expected %u, got %u",
 (unsigned int)drv_ctx.op_buf.buffer_size, (unsigned int)handle->size);
 return OMX_ErrorBadParameter;
 }
 }

            drv_ctx.op_buf.buffer_size = handle->size;

 if (!m_use_android_native_buffers) {
 if (!secure_mode) {
                    buff = (OMX_U8*)mmap(0, handle->size,
                            PROT_READ|PROT_WRITE, MAP_SHARED, handle->fd, 0);
 if (buff == MAP_FAILED) {
                        DEBUG_PRINT_ERROR("Failed to mmap pmem with fd = %d, size = %d", handle->fd, handle->size);
 return OMX_ErrorInsufficientResources;
 }
 }
 }
#if defined(_ANDROID_ICS_)
            native_buffer[i].nativehandle = handle;
            native_buffer[i].privatehandle = handle;
#endif
 if (!handle) {
                DEBUG_PRINT_ERROR("Native Buffer handle is NULL");
 return OMX_ErrorBadParameter;
 }
            drv_ctx.ptr_outputbuffer[i].pmem_fd = handle->fd;
            drv_ctx.ptr_outputbuffer[i].offset = 0;
            drv_ctx.ptr_outputbuffer[i].bufferaddr = buff;
            drv_ctx.ptr_outputbuffer[i].buffer_len = drv_ctx.op_buf.buffer_size;
            drv_ctx.ptr_outputbuffer[i].mmaped_size = handle->size;
 } else
#endif

 if (!ouput_egl_buffers && !m_use_output_pmem) {
#ifdef USE_ION
                drv_ctx.op_buf_ion_info[i].ion_device_fd = alloc_map_ion_memory(
                        drv_ctx.op_buf.buffer_size,drv_ctx.op_buf.alignment,
 &drv_ctx.op_buf_ion_info[i].ion_alloc_data,
 &drv_ctx.op_buf_ion_info[i].fd_ion_data, secure_mode ? ION_SECURE : 0);
 if (drv_ctx.op_buf_ion_info[i].ion_device_fd < 0) {
                    DEBUG_PRINT_ERROR("ION device fd is bad %d", drv_ctx.op_buf_ion_info[i].ion_device_fd);
 return OMX_ErrorInsufficientResources;
 }
                drv_ctx.ptr_outputbuffer[i].pmem_fd = \
                                      drv_ctx.op_buf_ion_info[i].fd_ion_data.fd;
#else
                drv_ctx.ptr_outputbuffer[i].pmem_fd = \
                                      open (MEM_DEVICE,O_RDWR);

 if (drv_ctx.ptr_outputbuffer[i].pmem_fd < 0) {
                    DEBUG_PRINT_ERROR("ION/pmem buffer fd is bad %d", drv_ctx.ptr_outputbuffer[i].pmem_fd);
 return OMX_ErrorInsufficientResources;
 }

  
 if (drv_ctx.ptr_outputbuffer[i].pmem_fd == 0) {
                    drv_ctx.ptr_outputbuffer[i].pmem_fd = \
                                          open (MEM_DEVICE,O_RDWR);
 if (drv_ctx.ptr_outputbuffer[i].pmem_fd < 0) {
                        DEBUG_PRINT_ERROR("ION/pmem buffer fd is bad %d", drv_ctx.ptr_outputbuffer[i].pmem_fd);
 return OMX_ErrorInsufficientResources;
 }
 }

 if (!align_pmem_buffers(drv_ctx.ptr_outputbuffer[i].pmem_fd,
                            drv_ctx.op_buf.buffer_size,
                            drv_ctx.op_buf.alignment)) {
                    DEBUG_PRINT_ERROR("align_pmem_buffers() failed");
                    close(drv_ctx.ptr_outputbuffer[i].pmem_fd);
 return OMX_ErrorInsufficientResources;
 }
#endif
 if (!secure_mode) {
                    drv_ctx.ptr_outputbuffer[i].bufferaddr =
 (unsigned char *)mmap(NULL, drv_ctx.op_buf.buffer_size,
                                PROT_READ|PROT_WRITE, MAP_SHARED,
                                drv_ctx.ptr_outputbuffer[i].pmem_fd,0);
 if (drv_ctx.ptr_outputbuffer[i].bufferaddr == MAP_FAILED) {
                        close(drv_ctx.ptr_outputbuffer[i].pmem_fd);
#ifdef USE_ION
                        free_ion_memory(&drv_ctx.op_buf_ion_info[i]);
#endif
                        DEBUG_PRINT_ERROR("Unable to mmap output buffer");
 return OMX_ErrorInsufficientResources;
 }
 }
                drv_ctx.ptr_outputbuffer[i].offset = 0;
                privateAppData = appData;
 } else {

                DEBUG_PRINT_LOW("Use_op_buf: out_pmem=%d",m_use_output_pmem);
 if (!appData || !bytes ) {
 if (!secure_mode && !buffer) {
                        DEBUG_PRINT_ERROR("Bad parameters for use buffer in EGL image case");
 return OMX_ErrorBadParameter;
 }
 }

                OMX_QCOM_PLATFORM_PRIVATE_LIST *pmem_list;
                OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO *pmem_info;
                pmem_list = (OMX_QCOM_PLATFORM_PRIVATE_LIST*) appData;
 if (!pmem_list || !pmem_list->entryList || !pmem_list->entryList->entry ||
 !pmem_list->nEntries ||
                        pmem_list->entryList->type != OMX_QCOM_PLATFORM_PRIVATE_PMEM) {
                    DEBUG_PRINT_ERROR("Pmem info not valid in use buffer");
 return OMX_ErrorBadParameter;
 }
                pmem_info = (OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO *)
                    pmem_list->entryList->entry;
                DEBUG_PRINT_LOW("vdec: use buf: pmem_fd=0x%lx",
                        pmem_info->pmem_fd);
                drv_ctx.ptr_outputbuffer[i].pmem_fd = pmem_info->pmem_fd;
                drv_ctx.ptr_outputbuffer[i].offset = pmem_info->offset;
                drv_ctx.ptr_outputbuffer[i].bufferaddr = buff;
                drv_ctx.ptr_outputbuffer[i].mmaped_size =
                    drv_ctx.ptr_outputbuffer[i].buffer_len = drv_ctx.op_buf.buffer_size;
                privateAppData = appData;
 }
        m_pmem_info[i].offset = drv_ctx.ptr_outputbuffer[i].offset;
        m_pmem_info[i].pmem_fd = drv_ctx.ptr_outputbuffer[i].pmem_fd;
        m_pmem_info[i].size = drv_ctx.ptr_outputbuffer[i].buffer_len;
        m_pmem_info[i].mapped_size = drv_ctx.ptr_outputbuffer[i].mmaped_size;
        m_pmem_info[i].buffer = drv_ctx.ptr_outputbuffer[i].bufferaddr;

 *bufferHdr = (m_out_mem_ptr + i );
 if (secure_mode)
            drv_ctx.ptr_outputbuffer[i].bufferaddr = *bufferHdr;
        memcpy (&setbuffers.buffer,&drv_ctx.ptr_outputbuffer[i],
 sizeof (vdec_bufferpayload));

        DEBUG_PRINT_HIGH("Set the Output Buffer Idx: %d Addr: %p, pmem_fd=0x%x", i,
                drv_ctx.ptr_outputbuffer[i].bufferaddr,
                drv_ctx.ptr_outputbuffer[i].pmem_fd );

        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_USERPTR;
        plane[0].length = drv_ctx.op_buf.buffer_size;
        plane[0].m.userptr = (unsigned long)drv_ctx.ptr_outputbuffer[i].bufferaddr -
 (unsigned long)drv_ctx.ptr_outputbuffer[i].offset;
        plane[0].reserved[0] = drv_ctx.ptr_outputbuffer[i].pmem_fd;
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
            DEBUG_PRINT_ERROR("Extradata index is more than allowed: %d", extra_idx);
 return OMX_ErrorBadParameter;
 }
        buf.m.planes = plane;
        buf.length = drv_ctx.num_planes;

 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_PREPARE_BUF, &buf)) {
            DEBUG_PRINT_ERROR("Failed to prepare bufs");
  
 return OMX_ErrorInsufficientResources;
 }

 if (i == (drv_ctx.op_buf.actualcount -1) && !streaming[CAPTURE_PORT]) {
 enum v4l2_buf_type buf_type;
            buf_type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
 if (ioctl(drv_ctx.video_driver_fd, VIDIOC_STREAMON,&buf_type)) {
 return OMX_ErrorInsufficientResources;
 } else {
                streaming[CAPTURE_PORT] = true;
                DEBUG_PRINT_LOW("STREAMON Successful");
 }
 }

 (*bufferHdr)->nAllocLen = drv_ctx.op_buf.buffer_size;
 if (m_enable_android_native_buffers) {
            DEBUG_PRINT_LOW("setting pBuffer to private_handle_t %p", handle);
 (*bufferHdr)->pBuffer = (OMX_U8 *)handle;
 } else {
 (*bufferHdr)->pBuffer = buff;
 }
 (*bufferHdr)->pAppPrivate = privateAppData;
        BITMASK_SET(&m_out_bm_count,i);
 }
 return eRet;
}
