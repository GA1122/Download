bool venc_dev::venc_use_buf(void *buf_addr, unsigned port,unsigned index)
{

 struct pmem *pmem_tmp;
 struct v4l2_buffer buf;
 struct v4l2_plane plane[VIDEO_MAX_PLANES];
 int rc = 0;
 unsigned int extra_idx;

    pmem_tmp = (struct pmem *)buf_addr;
    DEBUG_PRINT_LOW("venc_use_buf:: pmem_tmp = %p", pmem_tmp);

 if (port == PORT_INDEX_IN) {
        buf.index = index;
        buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        buf.memory = V4L2_MEMORY_USERPTR;
        plane[0].length = pmem_tmp->size;
        plane[0].m.userptr = (unsigned long)pmem_tmp->buffer;
        plane[0].reserved[0] = pmem_tmp->fd;
        plane[0].reserved[1] = 0;
        plane[0].data_offset = pmem_tmp->offset;
        buf.m.planes = plane;
        buf.length = 1;

        rc = ioctl(m_nDriver_fd, VIDIOC_PREPARE_BUF, &buf);

 if (rc)
            DEBUG_PRINT_LOW("VIDIOC_PREPARE_BUF Failed");
 } else if (port == PORT_INDEX_OUT) {
        extra_idx = EXTRADATA_IDX(num_planes);

 if ((num_planes > 1) && (extra_idx)) {
            rc = allocate_extradata();

 if (rc)
                DEBUG_PRINT_ERROR("Failed to allocate extradata: %d", rc);
 }

        buf.index = index;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_USERPTR;
        plane[0].length = pmem_tmp->size;
        plane[0].m.userptr = (unsigned long)pmem_tmp->buffer;
        plane[0].reserved[0] = pmem_tmp->fd;
        plane[0].reserved[1] = 0;
        plane[0].data_offset = pmem_tmp->offset;
        buf.m.planes = plane;
        buf.length = num_planes;

 if (extra_idx && (extra_idx < VIDEO_MAX_PLANES)) {
            plane[extra_idx].length = extradata_info.buffer_size;
            plane[extra_idx].m.userptr = (unsigned long) (extradata_info.uaddr + index * extradata_info.buffer_size);
#ifdef USE_ION
            plane[extra_idx].reserved[0] = extradata_info.ion.fd_ion_data.fd;
#endif
            plane[extra_idx].reserved[1] = extradata_info.buffer_size * index;
            plane[extra_idx].data_offset = 0;
 } else if (extra_idx >= VIDEO_MAX_PLANES) {
            DEBUG_PRINT_ERROR("Extradata index is more than allowed: %d", extra_idx);
 return OMX_ErrorBadParameter;
 }

        rc = ioctl(m_nDriver_fd, VIDIOC_PREPARE_BUF, &buf);

 if (rc)
            DEBUG_PRINT_LOW("VIDIOC_PREPARE_BUF Failed");
 } else {
        DEBUG_PRINT_ERROR("ERROR: venc_use_buf:Invalid Port Index ");
 return false;
 }

 return true;
}
