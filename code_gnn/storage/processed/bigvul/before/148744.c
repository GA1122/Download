bool V4L2JpegEncodeAccelerator::EncodedInstance::SetOutputBufferFormat(
    gfx::Size coded_size,
    size_t buffer_size) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  DCHECK(!output_streamon_);
  DCHECK(running_job_queue_.empty());

  struct v4l2_format format;
  memset(&format, 0, sizeof(format));
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  format.fmt.pix_mp.num_planes = kMaxJpegPlane;
  format.fmt.pix_mp.pixelformat = output_buffer_pixelformat_;
  format.fmt.pix_mp.field = V4L2_FIELD_ANY;
  format.fmt.pix_mp.plane_fmt[0].sizeimage = buffer_size;
  format.fmt.pix_mp.width = coded_size.width();
  format.fmt.pix_mp.height = coded_size.height();
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_S_FMT, &format);
  DCHECK_EQ(format.fmt.pix_mp.pixelformat, output_buffer_pixelformat_);

  return true;
}
