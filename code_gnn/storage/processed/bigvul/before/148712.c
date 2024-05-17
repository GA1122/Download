bool V4L2JpegEncodeAccelerator::EncodedInstance::EnqueueOutputRecord() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  DCHECK(!free_output_buffers_.empty());

  const int index = free_output_buffers_.back();
  JpegBufferRecord& output_record = output_buffer_map_[index];
  DCHECK(!output_record.at_device);
  struct v4l2_buffer qbuf;
  struct v4l2_plane planes[kMaxJpegPlane];
  memset(&qbuf, 0, sizeof(qbuf));
  memset(planes, 0, sizeof(planes));
  qbuf.index = index;
  qbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  qbuf.memory = V4L2_MEMORY_MMAP;
  qbuf.length = base::size(planes);
  qbuf.m.planes = planes;
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_QBUF, &qbuf);
  output_record.at_device = true;
  free_output_buffers_.pop_back();
  return true;
}
