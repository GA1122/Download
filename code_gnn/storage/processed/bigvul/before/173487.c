int release_buffers(omx_vdec* obj, enum vdec_buffer buffer_type)
{
 struct v4l2_requestbuffers bufreq;
 int rc = 0;
 if (buffer_type == VDEC_BUFFER_TYPE_OUTPUT) {
        bufreq.memory = V4L2_MEMORY_USERPTR;
        bufreq.count = 0;
        bufreq.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        rc = ioctl(obj->drv_ctx.video_driver_fd,VIDIOC_REQBUFS, &bufreq);
 } else if(buffer_type == VDEC_BUFFER_TYPE_INPUT) {
        bufreq.memory = V4L2_MEMORY_USERPTR;
        bufreq.count = 0;
        bufreq.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        rc = ioctl(obj->drv_ctx.video_driver_fd,VIDIOC_REQBUFS, &bufreq);
 }
 return rc;
}
