void DrawingBuffer::MailboxReleasedGpu(RefPtr<ColorBuffer> color_buffer,
                                       const gpu::SyncToken& sync_token,
                                       bool lost_resource) {
  if (color_buffer == front_color_buffer_)
    front_color_buffer_ = nullptr;

  color_buffer->receive_sync_token = sync_token;

  if (destruction_in_progress_ || color_buffer->size != size_ ||
      gl_->GetGraphicsResetStatusKHR() != GL_NO_ERROR || lost_resource ||
      is_hidden_) {
    return;
  }

  size_t cache_limit = 1;
  if (ShouldUseChromiumImage())
    cache_limit = 4;
  while (recycled_color_buffer_queue_.size() >= cache_limit)
    recycled_color_buffer_queue_.TakeLast();

  recycled_color_buffer_queue_.push_front(color_buffer);
}
