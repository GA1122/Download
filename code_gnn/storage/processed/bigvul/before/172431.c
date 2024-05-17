bool venc_dev::venc_fill_buf(void *buffer, void *pmem_data_buf,unsigned index,unsigned fd)
{
 struct pmem *temp_buffer = NULL;
 struct venc_buffer  frameinfo;
 struct v4l2_buffer buf;
 struct v4l2_plane plane[VIDEO_MAX_PLANES];
 int rc = 0;
 unsigned int extra_idx;
 struct OMX_BUFFERHEADERTYPE *bufhdr;

 if (buffer == NULL)
 return false;

    bufhdr = (OMX_BUFFERHEADERTYPE *)buffer;

 if (pmem_data_buf) {
        DEBUG_PRINT_LOW("Internal PMEM addr for o/p Heap UseBuf: %p", pmem_data_buf);
        plane[0].m.userptr = (unsigned long)pmem_data_buf;
 } else {
        DEBUG_PRINT_LOW("Shared PMEM addr for o/p PMEM UseBuf/AllocateBuf: %p", bufhdr->pBuffer);
        plane[0].m.userptr = (unsigned long)bufhdr->pBuffer;
 }

    buf.index = index;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_USERPTR;
    plane[0].length = bufhdr->nAllocLen;
    plane[0].bytesused = bufhdr->nFilledLen;
    plane[0].reserved[0] = fd;
    plane[0].reserved[1] = 0;
    plane[0].data_offset = bufhdr->nOffset;
    buf.m.planes = plane;
    buf.length = num_planes;

    extra_idx = EXTRADATA_IDX(num_planes);

 if (extra_idx && (extra_idx < VIDEO_MAX_PLANES)) {
        plane[extra_idx].bytesused = 0;
        plane[extra_idx].length = extradata_info.buffer_size;
        plane[extra_idx].m.userptr = (unsigned long) (extradata_info.uaddr + index * extradata_info.buffer_size);
#ifdef USE_ION
        plane[extra_idx].reserved[0] = extradata_info.ion.fd_ion_data.fd;
#endif
        plane[extra_idx].reserved[1] = extradata_info.buffer_size * index;
        plane[extra_idx].data_offset = 0;
 } else if (extra_idx >= VIDEO_MAX_PLANES) {
        DEBUG_PRINT_ERROR("Extradata index higher than expected: %d", extra_idx);
 return false;
 }

    rc = ioctl(m_nDriver_fd, VIDIOC_QBUF, &buf);

 if (rc) {
        DEBUG_PRINT_ERROR("Failed to qbuf (ftb) to driver");
 return false;
 }

    ftb++;
 return true;
}
