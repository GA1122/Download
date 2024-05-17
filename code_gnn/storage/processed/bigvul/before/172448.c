bool venc_dev::venc_reconfig_reqbufs()
{
 struct v4l2_requestbuffers bufreq;

    bufreq.memory = V4L2_MEMORY_USERPTR;
    bufreq.count = m_sInput_buff_property.actualcount;
    bufreq.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
 if(ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq)) {
            DEBUG_PRINT_ERROR("VIDIOC_REQBUFS OUTPUT_MPLANE Failed when resume");
 return false;
 }

    bufreq.memory = V4L2_MEMORY_USERPTR;
    bufreq.count = m_sOutput_buff_property.actualcount;
    bufreq.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
 if(ioctl(m_nDriver_fd,VIDIOC_REQBUFS, &bufreq))
 {
            DEBUG_PRINT_ERROR("ERROR: Request for setting o/p buffer count failed when resume");
 return false;
 }
 return true;
}
