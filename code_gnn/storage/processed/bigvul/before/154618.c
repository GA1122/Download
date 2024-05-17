error::Error GLES2DecoderPassthroughImpl::DoDeleteFramebuffers(
    GLsizei n,
    const volatile GLuint* framebuffers) {
  if (n < 0) {
    InsertError(GL_INVALID_VALUE, "n cannot be negative.");
    return error::kNoError;
  }

  std::vector<GLuint> framebuffers_copy(framebuffers, framebuffers + n);

  for (GLuint framebuffer : framebuffers_copy) {
    if (framebuffer == bound_draw_framebuffer_) {
      bound_draw_framebuffer_ = 0;
      if (emulated_back_buffer_) {
        api()->glBindFramebufferEXTFn(
            GL_DRAW_FRAMEBUFFER, emulated_back_buffer_->framebuffer_service_id);
      }

      ApplySurfaceDrawOffset();
    }
    if (framebuffer == bound_read_framebuffer_) {
      bound_read_framebuffer_ = 0;
      if (emulated_back_buffer_) {
        api()->glBindFramebufferEXTFn(
            GL_READ_FRAMEBUFFER, emulated_back_buffer_->framebuffer_service_id);
      }
    }
  }

  return DeleteHelper(n, framebuffers_copy.data(), &framebuffer_id_map_,
                      [this](GLsizei n, GLuint* framebuffers) {
                        api()->glDeleteFramebuffersEXTFn(n, framebuffers);
                      });
}
