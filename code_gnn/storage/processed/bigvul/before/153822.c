void GLES2Implementation::SwapBuffers(uint64_t swap_id, GLbitfield flags) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glSwapBuffers()");
  swap_buffers_tokens_.push(helper_->InsertToken());
  helper_->SwapBuffers(swap_id, flags);
  helper_->CommandBufferHelper::Flush();
  if (swap_buffers_tokens_.size() > kMaxSwapBuffers + 1) {
    helper_->WaitForToken(swap_buffers_tokens_.front());
    swap_buffers_tokens_.pop();
  }
}
