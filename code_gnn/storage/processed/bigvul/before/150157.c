size_t ImageProcessorClient::GetErrorCount() const {
  base::AutoLock auto_lock_(output_lock_);
  return image_processor_error_count_;
}
