void GLES2Implementation::SwapBuffersWithBoundsCHROMIUM(uint64_t swap_id,
                                                        GLsizei count,
                                                        const GLint* rects,
                                                        GLbitfield flags) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glSwapBuffersWithBoundsCHROMIUM("
                     << count << ", " << static_cast<const void*>(rects)
                     << ")");
  GPU_CLIENT_LOG_CODE_BLOCK({
    for (GLsizei i = 0; i < count; ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << rects[0 + i * 4] << ", "
                          << rects[1 + i * 4] << ", " << rects[2 + i * 4]
                          << ", " << rects[3 + i * 4]);
    }
  });
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, "glSwapBuffersWithBoundsCHROMIUM",
               "count < 0");
    return;
  }

  swap_buffers_tokens_.push(helper_->InsertToken());
  helper_->SwapBuffersWithBoundsCHROMIUMImmediate(swap_id, count, rects, flags);
  helper_->CommandBufferHelper::Flush();
  if (swap_buffers_tokens_.size() > kMaxSwapBuffers + 1) {
    helper_->WaitForToken(swap_buffers_tokens_.front());
    swap_buffers_tokens_.pop();
  }
}
