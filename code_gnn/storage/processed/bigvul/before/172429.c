bool venc_dev::venc_empty_buf(void *buffer, void *pmem_data_buf, unsigned index, unsigned fd)
{
 struct pmem *temp_buffer;
 struct v4l2_buffer buf;
 struct v4l2_plane plane;
 int rc=0;
 struct OMX_BUFFERHEADERTYPE *bufhdr;
 struct v4l2_control control;
    encoder_media_buffer_type * meta_buf = NULL;
    temp_buffer = (struct pmem *)buffer;

    memset (&buf, 0, sizeof(buf));
    memset (&plane, 0, sizeof(plane));

 if (buffer == NULL) {
        DEBUG_PRINT_ERROR("ERROR: venc_etb: buffer is NULL");
 return false;
 }

    bufhdr = (OMX_BUFFERHEADERTYPE *)buffer;

    DEBUG_PRINT_LOW("Input buffer length %u", (unsigned int)bufhdr->nFilledLen);

 if (pmem_data_buf) {
        DEBUG_PRINT_LOW("Internal PMEM addr for i/p Heap UseBuf: %p", pmem_data_buf);
        plane.m.userptr = (unsigned long)pmem_data_buf;
        plane.data_offset = bufhdr->nOffset;
        plane.length = bufhdr->nAllocLen;
        plane.bytesused = bufhdr->nFilledLen;
 } else {
 if (metadatamode) {
            plane.m.userptr = index;
            meta_buf = (encoder_media_buffer_type *)bufhdr->pBuffer;

 if (!meta_buf) {
 if (!bufhdr->nFilledLen && (bufhdr->nFlags & OMX_BUFFERFLAG_EOS)) {
                    plane.data_offset = bufhdr->nOffset;
                    plane.length = bufhdr->nAllocLen;
                    plane.bytesused = bufhdr->nFilledLen;
                    DEBUG_PRINT_LOW("venc_empty_buf: empty EOS buffer");
 } else {
 return false;
 }
 } else if (!color_format) {
 int color_space = 0;

 if (meta_buf->buffer_type == kMetadataBufferTypeCameraSource) {
 if (meta_buf->meta_handle->numFds + meta_buf->meta_handle->numInts > 3 &&
                        meta_buf->meta_handle->data[3] & private_handle_t::PRIV_FLAGS_ITU_R_709) {
                        buf.flags = V4L2_MSM_BUF_FLAG_YUV_601_709_CLAMP;
			color_space = V4L2_COLORSPACE_REC709;
 }

 if (meta_buf->meta_handle->numFds + meta_buf->meta_handle->numInts > 2) {
                    plane.data_offset = meta_buf->meta_handle->data[1];
                    plane.length = meta_buf->meta_handle->data[2];
                    plane.bytesused = meta_buf->meta_handle->data[2];
 }
 if (!camera_mode_enabled) {
                        camera_mode_enabled = true;
                        control.id = V4L2_CID_MPEG_VIDC_SET_PERF_LEVEL;
                        control.value = V4L2_CID_MPEG_VIDC_PERF_LEVEL_NOMINAL;
                        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc)
                            DEBUG_PRINT_HIGH("Failed to set control for perf level");
                        DEBUG_PRINT_LOW("Set control id = 0x%x, value = 0x%x, meta_buf type = %d",
                                control.id, control.value, meta_buf->buffer_type);
 }
                    DEBUG_PRINT_LOW("venc_empty_buf: camera buf: fd = %d filled %d of %d flag 0x%x",
                            fd, plane.bytesused, plane.length, buf.flags);
 } else if (meta_buf->buffer_type == kMetadataBufferTypeGrallocSource) {
 private_handle_t *handle = (private_handle_t *)meta_buf->meta_handle;
                    fd = handle->fd;
                    plane.data_offset = 0;
                    plane.length = handle->size;
                    plane.bytesused = handle->size;
 if ((!camera_mode_enabled) && (handle->flags & private_handle_t:: PRIV_FLAGS_CAMERA_WRITE)) {
                        camera_mode_enabled = true;
                        control.id = V4L2_CID_MPEG_VIDC_SET_PERF_LEVEL;
                        control.value = V4L2_CID_MPEG_VIDC_PERF_LEVEL_NOMINAL;
                        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);
 if (rc)
                            DEBUG_PRINT_HIGH("Failed to set perl level");
                        DEBUG_PRINT_LOW("Set control id = 0x%x, value = 0x%x, flags = 0x%x",
                                control.id, control.value, handle->flags);
 }

 if (handle->base_metadata) {
 MetaData_t *pMeta =
 reinterpret_cast<MetaData_t*>(handle->base_metadata);
 ColorSpace_t csc = pMeta->operation & UPDATE_COLOR_SPACE ?
                                pMeta->colorSpace : (ColorSpace_t)-1;
 if (csc == ITU_R_709) {
                            buf.flags |= V4L2_MSM_BUF_FLAG_YUV_601_709_CLAMP;
                            DEBUG_PRINT_LOW("venc_empty_buf: force 601 -> 709 clamping");
			    color_space = V4L2_COLORSPACE_REC709;
 } else if (csc == ITU_R_601_FR) {
                            DEBUG_PRINT_LOW("venc_empty_buf: 601 full-range");
                            color_space = V4L2_COLORSPACE_470_SYSTEM_BG;
 } else if (csc == ITU_R_601) {
                            DEBUG_PRINT_LOW("venc_empty_buf: 601 clamped");
                            color_space = V4L2_COLORSPACE_BT878;
 }
 } else {
                        DEBUG_PRINT_LOW("venc_empty_buf: gralloc metadata is NULL");
 }
                        DEBUG_PRINT_LOW("venc_empty_buf: Opaque camera buf: fd = %d "
 ": filled %d of %d", fd, plane.bytesused, plane.length);
 }


 if (!streaming[OUTPUT_PORT]) {
 struct v4l2_format fmt;
			fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
			fmt.fmt.pix_mp.pixelformat = m_sVenc_cfg.inputformat;
			fmt.fmt.pix_mp.colorspace = static_cast<decltype(fmt.fmt.pix_mp.colorspace)>(color_space);
			fmt.fmt.pix_mp.height = m_sVenc_cfg.input_height;
			fmt.fmt.pix_mp.width = m_sVenc_cfg.input_width;
 if (ioctl(m_nDriver_fd, VIDIOC_S_FMT, &fmt)) {
				DEBUG_PRINT_ERROR("Failed setting color format in in etb %x", m_sVenc_cfg.inputformat);
 return false;
 }
 }
 } else {
                plane.data_offset = bufhdr->nOffset;
                plane.length = bufhdr->nAllocLen;
                plane.bytesused = bufhdr->nFilledLen;
                DEBUG_PRINT_LOW("venc_empty_buf: Opaque non-camera buf: fd = %d "
 ": filled %d of %d", fd, plane.bytesused, plane.length);
 }
 } else {
            plane.data_offset = bufhdr->nOffset;
            plane.length = bufhdr->nAllocLen;
            plane.bytesused = bufhdr->nFilledLen;
            DEBUG_PRINT_LOW("venc_empty_buf: non-camera buf: fd = %d filled %d of %d",
                    fd, plane.bytesused, plane.length);
 }
 }

    buf.index = index;
    buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    buf.memory = V4L2_MEMORY_USERPTR;
    plane.reserved[0] = fd;
    plane.reserved[1] = 0;
    buf.m.planes = &plane;
    buf.length = 1;

 if (bufhdr->nFlags & OMX_BUFFERFLAG_EOS)
        buf.flags |= V4L2_QCOM_BUF_FLAG_EOS;

    buf.timestamp.tv_sec = bufhdr->nTimeStamp / 1000000;
    buf.timestamp.tv_usec = (bufhdr->nTimeStamp % 1000000);
    rc = ioctl(m_nDriver_fd, VIDIOC_QBUF, &buf);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to qbuf (etb) to driver");
 return false;
 }

    etb++;

 if (!streaming[OUTPUT_PORT]) {
 enum v4l2_buf_type buf_type;
        buf_type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
 int ret;
        ret = ioctl(m_nDriver_fd, VIDIOC_STREAMON, &buf_type);

 if (ret) {
            DEBUG_PRINT_ERROR("Failed to call streamon");
 if (errno == EBUSY) {
                hw_overload = true;
 }
 return false;
 } else {
            streaming[OUTPUT_PORT] = true;
 }
 }
 if (m_debug.in_buffer_log) {
        venc_input_log_buffers(bufhdr, fd, plane.data_offset);
 }

 return true;
}
