scoped_refptr<VideoFrame> ImageProcessorClient::CreateInputFrame(
    const Image& input_image) const {
  DCHECK_CALLED_ON_VALID_THREAD(test_main_thread_checker_);
  LOG_ASSERT(image_processor_);
  LOG_ASSERT(input_image.IsLoaded());
  LOG_ASSERT(input_image.DataSize() ==
             VideoFrame::AllocationSize(input_image.PixelFormat(),
                                        input_image.Size()));

  const auto format = input_image.PixelFormat();
  const auto visible_size = input_image.Size();

  const size_t num_planes = VideoFrame::NumPlanes(format);
  std::vector<VideoFrameLayout::Plane> planes(num_planes);
  const auto strides = VideoFrame::ComputeStrides(format, visible_size);
  size_t offset = 0;
  for (size_t i = 0; i < num_planes; ++i) {
    planes[i].stride = strides[i];
    planes[i].offset = offset;
    offset += VideoFrame::PlaneSize(format, i, visible_size).GetArea();
  }

  auto layout = VideoFrameLayout::CreateWithPlanes(
      format, visible_size, std::move(planes), {input_image.DataSize()});
  if (!layout) {
    LOG(ERROR) << "Failed to create VideoFrameLayout";
    return nullptr;
  }

  auto frame = VideoFrame::WrapExternalDataWithLayout(
      *layout, gfx::Rect(visible_size), visible_size, input_image.Data(),
      input_image.DataSize(), base::TimeDelta());
  if (!frame) {
    LOG(ERROR) << "Failed to create VideoFrame";
    return nullptr;
  }

  const auto& input_layout = image_processor_->input_layout();
  if (VideoFrame::IsStorageTypeMappable(
          image_processor_->input_storage_type())) {
    return CloneVideoFrameWithLayout(frame.get(), input_layout);
  } else {
#if defined(OS_CHROMEOS)
    LOG_ASSERT(image_processor_->input_storage_type() ==
               VideoFrame::STORAGE_DMABUFS);
#endif
    NOTIMPLEMENTED();
    return nullptr;
  }
}
