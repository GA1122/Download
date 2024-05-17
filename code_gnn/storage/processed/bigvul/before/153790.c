void GLES2Implementation::PostSubBufferCHROMIUM(uint64_t swap_id,
                                                GLint x,
                                                GLint y,
                                                GLint width,
                                                GLint height,
                                                GLbitfield flags) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] PostSubBufferCHROMIUM(" << x
                     << ", " << y << ", " << width << ", " << height << ")");
  TRACE_EVENT2("gpu", "GLES2::PostSubBufferCHROMIUM", "width", width, "height",
               height);

  swap_buffers_tokens_.push(helper_->InsertToken());
  helper_->PostSubBufferCHROMIUM(swap_id, x, y, width, height, flags);
  helper_->CommandBufferHelper::Flush();
  if (swap_buffers_tokens_.size() > kMaxSwapBuffers + 1) {
    helper_->WaitForToken(swap_buffers_tokens_.front());
    swap_buffers_tokens_.pop();
  }
}
