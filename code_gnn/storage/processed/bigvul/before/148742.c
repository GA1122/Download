bool V4L2JpegEncodeAccelerator::EncodedInstance::SetInputBufferFormat(
    gfx::Size coded_size) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  DCHECK(!input_streamon_);
  DCHECK(input_job_queue_.empty());

  constexpr uint32_t input_pix_fmt_candidates[] = {
      V4L2_PIX_FMT_YUV420M,
      V4L2_PIX_FMT_YUV420,
  };

  struct v4l2_format format;
  input_buffer_pixelformat_ = 0;
  for (const auto input_pix_fmt : input_pix_fmt_candidates) {
    DCHECK_EQ(V4L2Device::V4L2PixFmtToVideoPixelFormat(input_pix_fmt),
              PIXEL_FORMAT_I420);
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    format.fmt.pix_mp.num_planes = kMaxI420Plane;
    format.fmt.pix_mp.pixelformat = input_pix_fmt;
    format.fmt.pix_mp.field = V4L2_FIELD_ANY;
    format.fmt.pix_mp.width = coded_size.width();
    format.fmt.pix_mp.height = coded_size.height();

    if (device_->Ioctl(VIDIOC_S_FMT, &format) == 0 &&
        format.fmt.pix_mp.pixelformat == input_pix_fmt) {
      input_buffer_pixelformat_ = format.fmt.pix_mp.pixelformat;
      input_buffer_num_planes_ = format.fmt.pix_mp.num_planes;
      input_buffer_height_ = format.fmt.pix_mp.height;
      break;
    }
  }

  if (input_buffer_pixelformat_ == 0) {
    VLOGF(1) << "Neither YUV420 nor YUV420M is supported.";
    return false;
  }

  if (format.fmt.pix_mp.width != static_cast<uint32_t>(coded_size.width()) ||
      format.fmt.pix_mp.height != static_cast<uint32_t>(coded_size.height())) {
    VLOGF(1) << "Width " << coded_size.width() << "->"
             << format.fmt.pix_mp.width << ",Height " << coded_size.height()
             << "->" << format.fmt.pix_mp.height;
    return false;
  }
  for (int i = 0; i < format.fmt.pix_mp.num_planes; i++) {
    bytes_per_line_[i] = format.fmt.pix_mp.plane_fmt[i].bytesperline;
    VLOGF(3) << "Bytes Per Line:" << bytes_per_line_[i];
  }

  return true;
}
