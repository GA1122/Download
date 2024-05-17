void V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::EnqueueOutput() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  while (running_job_queue_.size() > OutputBufferQueuedCount() &&
         !free_output_buffers_.empty()) {
    if (!EnqueueOutputRecord())
      return;
  }

  if (!output_streamon_ && OutputBufferQueuedCount()) {
    __u32 type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    IOCTL_OR_ERROR_RETURN(VIDIOC_STREAMON, &type);
    output_streamon_ = true;
  }
}
