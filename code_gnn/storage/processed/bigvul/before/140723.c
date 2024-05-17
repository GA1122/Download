error::Error GLES2DecoderImpl::DoClear(GLbitfield mask) {
  DCHECK(!ShouldDeferDraws());
  if (CheckBoundFramebuffersValid("glClear")) {
    ApplyDirtyState();
    ScopedRenderTo do_render(framebuffer_state_.bound_draw_framebuffer.get());
    if (workarounds().gl_clear_broken) {
      ScopedGLErrorSuppressor suppressor("GLES2DecoderImpl::ClearWorkaround",
                                         GetErrorState());
      if (!BoundFramebufferHasDepthAttachment())
        mask &= ~GL_DEPTH_BUFFER_BIT;
      if (!BoundFramebufferHasStencilAttachment())
        mask &= ~GL_STENCIL_BUFFER_BIT;
      clear_framebuffer_blit_->ClearFramebuffer(
          this, GetBoundReadFrameBufferSize(), mask, state_.color_clear_red,
          state_.color_clear_green, state_.color_clear_blue,
          state_.color_clear_alpha, state_.depth_clear, state_.stencil_clear);
      return error::kNoError;
    }
    glClear(mask);
  }
  return error::kNoError;
}
