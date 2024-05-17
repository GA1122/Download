void V4L2JpegEncodeAccelerator::EncodedInstance::EnqueueInput() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  while (!input_job_queue_.empty() && !free_input_buffers_.empty()) {
    if (!EnqueueInputRecord())
      return;
  }

  if (!input_streamon_ && InputBufferQueuedCount()) {
    __u32 type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    IOCTL_OR_ERROR_RETURN(VIDIOC_STREAMON, &type);
    input_streamon_ = true;
  }
}
