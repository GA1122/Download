void GLES2Implementation::CommitOverlayPlanesCHROMIUM(uint64_t swap_id,
                                                      uint32_t flags) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] CommitOverlayPlanesCHROMIUM()");
  TRACE_EVENT0("gpu", "GLES2::CommitOverlayPlanesCHROMIUM");

  swap_buffers_tokens_.push(helper_->InsertToken());
  helper_->CommitOverlayPlanesCHROMIUM(swap_id, flags);
  helper_->CommandBufferHelper::Flush();
  if (swap_buffers_tokens_.size() > kMaxSwapBuffers + 1) {
    helper_->WaitForToken(swap_buffers_tokens_.front());
    swap_buffers_tokens_.pop();
  }
}
