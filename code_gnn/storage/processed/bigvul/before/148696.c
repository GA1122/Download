void V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::DestroyOutputBuffers() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  free_output_buffers_.clear();

  if (output_streamon_) {
    __u32 type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    IOCTL_OR_ERROR_RETURN(VIDIOC_STREAMOFF, &type);
    output_streamon_ = false;
  }

  struct v4l2_requestbuffers reqbufs;
  memset(&reqbufs, 0, sizeof(reqbufs));
  reqbufs.count = 0;
  reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  reqbufs.memory = V4L2_MEMORY_DMABUF;
  IOCTL_OR_LOG_ERROR(VIDIOC_REQBUFS, &reqbufs);
}
