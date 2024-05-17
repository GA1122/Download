bool V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::RequestOutputBuffers() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  struct v4l2_requestbuffers reqbufs;
  memset(&reqbufs, 0, sizeof(reqbufs));
  reqbufs.count = kBufferCount;
  reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  reqbufs.memory = V4L2_MEMORY_DMABUF;
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_REQBUFS, &reqbufs);

  DCHECK(free_output_buffers_.empty());
  for (size_t i = 0; i < reqbufs.count; ++i) {
    free_output_buffers_.push_back(i);
  }

  return true;
}
