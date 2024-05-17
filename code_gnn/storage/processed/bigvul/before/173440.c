OMX_ERRORTYPE  omx_vdec::fill_this_buffer_proxy(
        OMX_IN OMX_HANDLETYPE        hComp,
        OMX_IN OMX_BUFFERHEADERTYPE* bufferAdd)
{
    OMX_ERRORTYPE nRet = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE *buffer = bufferAdd;
 unsigned nPortIndex = 0;
 struct vdec_fillbuffer_cmd fillbuffer;
 struct vdec_bufferpayload     *ptr_outputbuffer = NULL;
 struct vdec_output_frameinfo  *ptr_respbuffer = NULL;

    nPortIndex = buffer-((OMX_BUFFERHEADERTYPE *)client_buffers.get_il_buf_hdr());

 if (bufferAdd == NULL || nPortIndex > drv_ctx.op_buf.actualcount) {
        DEBUG_PRINT_ERROR("FTBProxy: ERROR: invalid buffer index, nPortIndex %u bufCount %u",
            nPortIndex, drv_ctx.op_buf.actualcount);
 return OMX_ErrorBadParameter;
 }

    DEBUG_PRINT_LOW("FTBProxy: bufhdr = %p, bufhdr->pBuffer = %p",
            bufferAdd, bufferAdd->pBuffer);
  
 if (m_out_bEnabled != OMX_TRUE || output_flush_progress == true) {
        DEBUG_PRINT_LOW("Output Buffers return flush/disable condition");
        buffer->nFilledLen = 0;
        m_cb.FillBufferDone (hComp,m_app_data,buffer);
 return OMX_ErrorNone;
 }

 if (dynamic_buf_mode) {
 if (!secure_mode) {
            drv_ctx.ptr_outputbuffer[nPortIndex].bufferaddr =
 (OMX_U8*)mmap(0, drv_ctx.op_buf.buffer_size,
                       PROT_READ|PROT_WRITE, MAP_SHARED,
                       drv_ctx.ptr_outputbuffer[nPortIndex].pmem_fd, 0);
 }
        drv_ctx.ptr_outputbuffer[nPortIndex].offset = 0;
        drv_ctx.ptr_outputbuffer[nPortIndex].buffer_len = drv_ctx.op_buf.buffer_size;
        drv_ctx.ptr_outputbuffer[nPortIndex].mmaped_size = drv_ctx.op_buf.buffer_size;
        buf_ref_add(drv_ctx.ptr_outputbuffer[nPortIndex].pmem_fd,
                    drv_ctx.ptr_outputbuffer[nPortIndex].offset);
 }

    pending_output_buffers++;
    buffer = client_buffers.get_dr_buf_hdr(bufferAdd);
 if (!buffer) {
       DEBUG_PRINT_ERROR("err: client_buffer ptr invalid");
 return OMX_ErrorBadParameter;
 }
    ptr_respbuffer = (struct vdec_output_frameinfo*)buffer->pOutputPortPrivate;
 if (ptr_respbuffer) {
        ptr_outputbuffer = (struct vdec_bufferpayload*)ptr_respbuffer->client_data;
 }

 if (ptr_respbuffer == NULL || ptr_outputbuffer == NULL) {
        DEBUG_PRINT_ERROR("resp buffer or outputbuffer is NULL");
        buffer->nFilledLen = 0;
        m_cb.FillBufferDone (hComp,m_app_data,buffer);
        pending_output_buffers--;
 return OMX_ErrorBadParameter;
 }

 int rc = 0;
 struct v4l2_buffer buf;
 struct v4l2_plane plane[VIDEO_MAX_PLANES];
    memset( (void *)&buf, 0, sizeof(buf));
    memset( (void *)plane, 0, (sizeof(struct v4l2_plane)*VIDEO_MAX_PLANES));
 unsigned int extra_idx = 0;

    buf.index = nPortIndex;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_USERPTR;
    plane[0].bytesused = buffer->nFilledLen;
    plane[0].length = drv_ctx.op_buf.buffer_size;
    plane[0].m.userptr =
 (unsigned long)drv_ctx.ptr_outputbuffer[nPortIndex].bufferaddr -
 (unsigned long)drv_ctx.ptr_outputbuffer[nPortIndex].offset;
    plane[0].reserved[0] = drv_ctx.ptr_outputbuffer[nPortIndex].pmem_fd;
    plane[0].reserved[1] = drv_ctx.ptr_outputbuffer[nPortIndex].offset;
    plane[0].data_offset = 0;
    extra_idx = EXTRADATA_IDX(drv_ctx.num_planes);
 if (extra_idx && (extra_idx < VIDEO_MAX_PLANES)) {
        plane[extra_idx].bytesused = 0;
        plane[extra_idx].length = drv_ctx.extradata_info.buffer_size;
        plane[extra_idx].m.userptr = (long unsigned int) (drv_ctx.extradata_info.uaddr + nPortIndex * drv_ctx.extradata_info.buffer_size);
#ifdef USE_ION
        plane[extra_idx].reserved[0] = drv_ctx.extradata_info.ion.fd_ion_data.fd;
#endif
        plane[extra_idx].reserved[1] = nPortIndex * drv_ctx.extradata_info.buffer_size;
        plane[extra_idx].data_offset = 0;
 } else if (extra_idx >= VIDEO_MAX_PLANES) {
        DEBUG_PRINT_ERROR("Extradata index higher than expected: %u", extra_idx);
 return OMX_ErrorBadParameter;
 }
    buf.m.planes = plane;
    buf.length = drv_ctx.num_planes;
    DEBUG_PRINT_LOW("SENDING FTB TO F/W - fd[0] = %d fd[1] = %d offset[1] = %d",
             plane[0].reserved[0],plane[extra_idx].reserved[0], plane[extra_idx].reserved[1]);

    rc = ioctl(drv_ctx.video_driver_fd, VIDIOC_QBUF, &buf);
 if (rc) {
  
        DEBUG_PRINT_ERROR("Failed to qbuf to driver");
 }
 return OMX_ErrorNone;
}
