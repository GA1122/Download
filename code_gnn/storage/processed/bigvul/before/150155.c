scoped_refptr<VideoFrame> ImageProcessorClient::CreateOutputFrame(
    const Image& output_image) const {
  DCHECK_CALLED_ON_VALID_THREAD(test_main_thread_checker_);
  LOG_ASSERT(output_image.IsMetadataLoaded());
  LOG_ASSERT(image_processor_);

  const auto& output_layout = image_processor_->output_layout();
  if (VideoFrame::IsStorageTypeMappable(
          image_processor_->input_storage_type())) {
    return VideoFrame::CreateFrameWithLayout(
        output_layout, gfx::Rect(output_image.Size()), output_image.Size(),
        base::TimeDelta(), false  );
  } else {
#if defined(OS_CHROMEOS)
    LOG_ASSERT(image_processor_->input_storage_type() ==
               VideoFrame::STORAGE_DMABUFS);
#endif
    NOTIMPLEMENTED();
    return nullptr;
  }
}
