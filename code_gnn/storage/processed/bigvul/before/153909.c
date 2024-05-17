void GLES2DecoderImpl::ClearUnclearedAttachments(
    GLenum target, Framebuffer* framebuffer) {
  framebuffer->ClearUnclearedIntOr3DTexturesOrPartiallyClearedTextures(
      this, texture_manager());

  bool cleared_int_renderbuffers = false;
  Framebuffer* draw_framebuffer = GetBoundDrawFramebuffer();
  if (framebuffer->HasUnclearedIntRenderbufferAttachments()) {
    if (target == GL_READ_FRAMEBUFFER && draw_framebuffer != framebuffer) {
      api()->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER,
                                    framebuffer->service_id());
    }
    state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
    ClearDeviceWindowRectangles();

    framebuffer->ClearUnclearedIntRenderbufferAttachments(
        renderbuffer_manager());

    cleared_int_renderbuffers = true;
  }

  GLbitfield clear_bits = 0;
  bool reset_draw_buffers = false;
  if (framebuffer->HasUnclearedColorAttachments()) {
    api()->glClearColorFn(0.0f, 0.0f, 0.0f, 0.0f);
    state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    clear_bits |= GL_COLOR_BUFFER_BIT;

    if (SupportsDrawBuffers()) {
      reset_draw_buffers =
          framebuffer->PrepareDrawBuffersForClearingUninitializedAttachments();
    }
  }

  if (framebuffer->HasUnclearedAttachment(GL_STENCIL_ATTACHMENT)) {
    api()->glClearStencilFn(0);
    state_.SetDeviceStencilMaskSeparate(GL_FRONT, kDefaultStencilMask);
    state_.SetDeviceStencilMaskSeparate(GL_BACK, kDefaultStencilMask);
    clear_bits |= GL_STENCIL_BUFFER_BIT;
  }

  if (framebuffer->HasUnclearedAttachment(GL_DEPTH_ATTACHMENT)) {
    api()->glClearDepthFn(1.0f);
    state_.SetDeviceDepthMask(GL_TRUE);
    clear_bits |= GL_DEPTH_BUFFER_BIT;
  }

  if (clear_bits) {
    if (!cleared_int_renderbuffers &&
        target == GL_READ_FRAMEBUFFER && draw_framebuffer != framebuffer) {
      api()->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER,
                                    framebuffer->service_id());
    }
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
    ClearDeviceWindowRectangles();
    if (workarounds().gl_clear_broken) {
      ClearFramebufferForWorkaround(clear_bits);
    } else {
      api()->glClearFn(clear_bits);
    }
  }

  if (cleared_int_renderbuffers || clear_bits) {
    if (reset_draw_buffers)
      framebuffer->RestoreDrawBuffers();
    RestoreClearState();
    if (target == GL_READ_FRAMEBUFFER && draw_framebuffer != framebuffer) {
      GLuint service_id = draw_framebuffer ? draw_framebuffer->service_id() :
                                             GetBackbufferServiceId();
      api()->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER, service_id);
    }
  }

  framebuffer_manager()->MarkAttachmentsAsCleared(
      framebuffer, renderbuffer_manager(), texture_manager());
}
