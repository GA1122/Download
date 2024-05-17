bool V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::SetInputBufferFormat(
    gfx::Size coded_size,
    const VideoFrameLayout& input_layout) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  DCHECK(!input_streamon_);
  DCHECK(input_job_queue_.empty());

  constexpr uint32_t input_pix_fmt_candidates[] = {V4L2_PIX_FMT_NV12M,
                                                   V4L2_PIX_FMT_NV12};

  struct v4l2_format format;
  input_buffer_pixelformat_ = 0;
  for (const auto input_pix_fmt : input_pix_fmt_candidates) {
    DCHECK_EQ(V4L2Device::V4L2PixFmtToVideoPixelFormat(input_pix_fmt),
              PIXEL_FORMAT_NV12);
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    format.fmt.pix_mp.num_planes = kMaxNV12Plane;
    format.fmt.pix_mp.pixelformat = input_pix_fmt;
    format.fmt.pix_mp.field = V4L2_FIELD_ANY;
    format.fmt.pix_mp.width = coded_size.width();
    format.fmt.pix_mp.height = coded_size.height();

    auto num_planes = input_layout.num_planes();
    for (size_t i = 0; i < num_planes; i++) {
      format.fmt.pix_mp.plane_fmt[i].sizeimage = input_layout.planes()[i].size;
      format.fmt.pix_mp.plane_fmt[i].bytesperline =
          input_layout.planes()[i].stride;
    }

    if (device_->Ioctl(VIDIOC_S_FMT, &format) == 0 &&
        format.fmt.pix_mp.pixelformat == input_pix_fmt) {
      device_input_layout_ = V4L2Device::V4L2FormatToVideoFrameLayout(format);

      input_buffer_pixelformat_ = format.fmt.pix_mp.pixelformat;
      input_buffer_num_planes_ = format.fmt.pix_mp.num_planes;
      input_buffer_height_ = format.fmt.pix_mp.height;
      break;
    }
  }

  if (input_buffer_pixelformat_ == 0) {
    VLOGF(1) << "Neither NV12 nor NV12M is supported.";
    return false;
  }

  if (format.fmt.pix_mp.width != static_cast<uint32_t>(coded_size.width()) ||
      format.fmt.pix_mp.height != static_cast<uint32_t>(coded_size.height())) {
    VLOGF(1) << "Width " << coded_size.width() << "->"
             << format.fmt.pix_mp.width << ",Height " << coded_size.height()
             << "->" << format.fmt.pix_mp.height;
    return false;
  }
  return true;
}
