void ImageProcessorClient::FrameReady(size_t frame_index,
                                      scoped_refptr<VideoFrame> frame) {
  DCHECK_CALLED_ON_VALID_THREAD(image_processor_client_thread_checker_);

  base::AutoLock auto_lock_(output_lock_);
  EXPECT_EQ(frame_index, num_processed_frames_);
  for (auto& processor : frame_processors_)
    processor->ProcessVideoFrame(std::move(frame), frame_index);
  num_processed_frames_++;
  output_cv_.Signal();
}
