bool V4L2JpegEncodeAccelerator::EncodedInstance::RequestInputBuffers() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  struct v4l2_format format;
  memset(&format, 0, sizeof(format));
  format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
  format.fmt.pix_mp.pixelformat = input_buffer_pixelformat_;
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_G_FMT, &format);

  struct v4l2_requestbuffers reqbufs;
  memset(&reqbufs, 0, sizeof(reqbufs));
  reqbufs.count = kBufferCount;
  reqbufs.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
  reqbufs.memory = V4L2_MEMORY_MMAP;
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_REQBUFS, &reqbufs);

  DCHECK(input_buffer_map_.empty());
  input_buffer_map_.resize(reqbufs.count);

  for (size_t i = 0; i < input_buffer_map_.size(); ++i) {
    free_input_buffers_.push_back(i);

    struct v4l2_buffer buffer;
    struct v4l2_plane planes[kMaxI420Plane];
    memset(&buffer, 0, sizeof(buffer));
    memset(planes, 0, sizeof(planes));
    buffer.index = i;
    buffer.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.m.planes = planes;
    buffer.length = base::size(planes);
    IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_QUERYBUF, &buffer);

    if (input_buffer_num_planes_ != buffer.length) {
      return false;
    }
    for (size_t j = 0; j < buffer.length; ++j) {
      if (base::checked_cast<int64_t>(planes[j].length) <
          VideoFrame::PlaneSize(
              PIXEL_FORMAT_I420, j,
              gfx::Size(format.fmt.pix_mp.width, format.fmt.pix_mp.height))
              .GetArea()) {
        return false;
      }
      void* address =
          device_->Mmap(NULL, planes[j].length, PROT_READ | PROT_WRITE,
                        MAP_SHARED, planes[j].m.mem_offset);
      if (address == MAP_FAILED) {
        VPLOGF(1) << "mmap() failed";
        return false;
      }
      input_buffer_map_[i].address[j] = address;
      input_buffer_map_[i].length[j] = planes[j].length;
    }
  }

  return true;
}
