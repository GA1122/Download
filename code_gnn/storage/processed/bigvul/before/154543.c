ScopedFramebufferBindingReset::~ScopedFramebufferBindingReset() {
  if (supports_separate_fbo_bindings_) {
    api_->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER, draw_framebuffer_);
    api_->glBindFramebufferEXTFn(GL_READ_FRAMEBUFFER, read_framebuffer_);
  } else {
    api_->glBindFramebufferEXTFn(GL_FRAMEBUFFER, draw_framebuffer_);
  }
}
