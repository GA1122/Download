bool GLES2DecoderImpl::CheckFramebufferValid(
    Framebuffer* framebuffer,
    GLenum target,
    GLenum gl_error,
    const char* func_name) {
  if (!framebuffer) {
    if (surfaceless_)
      return false;
    if (backbuffer_needs_clear_bits_) {
      api()->glClearColorFn(0, 0, 0, BackBufferAlphaClearColor());
      state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      api()->glClearStencilFn(0);
      state_.SetDeviceStencilMaskSeparate(GL_FRONT, kDefaultStencilMask);
      state_.SetDeviceStencilMaskSeparate(GL_BACK, kDefaultStencilMask);
      api()->glClearDepthFn(1.0f);
      state_.SetDeviceDepthMask(GL_TRUE);
      state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
      ClearDeviceWindowRectangles();
      bool reset_draw_buffer = false;
      if ((backbuffer_needs_clear_bits_ & GL_COLOR_BUFFER_BIT) != 0 &&
          back_buffer_draw_buffer_ == GL_NONE) {
        reset_draw_buffer = true;
        GLenum buf = GL_BACK;
        if (GetBackbufferServiceId() != 0)   
          buf = GL_COLOR_ATTACHMENT0;
        api()->glDrawBuffersARBFn(1, &buf);
      }
      if (workarounds().gl_clear_broken) {
        ClearFramebufferForWorkaround(backbuffer_needs_clear_bits_);
      } else {
        api()->glClearFn(backbuffer_needs_clear_bits_);
      }
      if (reset_draw_buffer) {
        GLenum buf = GL_NONE;
        api()->glDrawBuffersARBFn(1, &buf);
      }
      backbuffer_needs_clear_bits_ = 0;
      RestoreClearState();
    }
    return true;
  }

  if (!framebuffer_manager()->IsComplete(framebuffer)) {
    GLenum completeness = framebuffer->IsPossiblyComplete(feature_info_.get());
    if (completeness != GL_FRAMEBUFFER_COMPLETE) {
      LOCAL_SET_GL_ERROR(gl_error, func_name, "framebuffer incomplete");
      return false;
    }

    if (framebuffer->GetStatus(texture_manager(), target) !=
        GL_FRAMEBUFFER_COMPLETE) {
      LOCAL_SET_GL_ERROR(
          gl_error, func_name, "framebuffer incomplete (check)");
      return false;
    }
    framebuffer_manager()->MarkAsComplete(framebuffer);
  }

  if (renderbuffer_manager()->HaveUnclearedRenderbuffers() ||
      texture_manager()->HaveUnclearedMips()) {
    if (!framebuffer->IsCleared()) {
      ClearUnclearedAttachments(target, framebuffer);
    }
  }
  return true;
}
