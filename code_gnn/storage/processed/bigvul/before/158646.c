bool BackRenderbuffer::AllocateStorage(const gfx::Size& size,
                                       GLenum format,
                                       GLsizei samples) {
  ScopedGLErrorSuppressor suppressor("BackRenderbuffer::AllocateStorage",
                                     decoder_->state_.GetErrorState());
  ScopedRenderBufferBinder binder(&decoder_->state_, id_);

  uint32_t estimated_size = 0;
  if (!decoder_->renderbuffer_manager()->ComputeEstimatedRenderbufferSize(
          size.width(), size.height(), samples, format, &estimated_size)) {
    return false;
  }

  decoder_->RenderbufferStorageMultisampleHelper(GL_RENDERBUFFER, samples,
                                                 format, size.width(),
                                                 size.height(), kDoNotForce);

  bool alpha_channel_needs_clear =
      (format == GL_RGBA || format == GL_RGBA8) &&
      !decoder_->offscreen_buffer_should_have_alpha_;
  if (alpha_channel_needs_clear) {
    GLuint fbo;
    api()->glGenFramebuffersEXTFn(1, &fbo);
    {
      ScopedFramebufferBinder binder(decoder_, fbo);
      api()->glFramebufferRenderbufferEXTFn(
          GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, id_);
      api()->glClearColorFn(0, 0, 0, decoder_->BackBufferAlphaClearColor());
      decoder_->state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      decoder_->state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
      decoder_->ClearDeviceWindowRectangles();
      api()->glClearFn(GL_COLOR_BUFFER_BIT);
      decoder_->RestoreClearState();
    }
    api()->glDeleteFramebuffersEXTFn(1, &fbo);
  }

  bool success = api()->glGetErrorFn() == GL_NO_ERROR;
  if (success) {
    memory_tracker_.TrackMemFree(bytes_allocated_);
    bytes_allocated_ = estimated_size;
    memory_tracker_.TrackMemAlloc(bytes_allocated_);
  }
  return success;
}
