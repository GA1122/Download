error::Error GLES2DecoderImpl::DoClear(GLbitfield mask) {
  const char* func_name = "glClear";
  DCHECK(!ShouldDeferDraws());
  if (mask &
      ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "invalid mask");
    return error::kNoError;
  }
  if (CheckBoundDrawFramebufferValid(func_name)) {
    ApplyDirtyState();
    if (workarounds().gl_clear_broken) {
      if (!BoundFramebufferHasDepthAttachment())
        mask &= ~GL_DEPTH_BUFFER_BIT;
      if (!BoundFramebufferHasStencilAttachment())
        mask &= ~GL_STENCIL_BUFFER_BIT;
      ClearFramebufferForWorkaround(mask);
      return error::kNoError;
    }
    if (mask & GL_COLOR_BUFFER_BIT) {
      Framebuffer* framebuffer =
          framebuffer_state_.bound_draw_framebuffer.get();
      if (framebuffer && framebuffer->ContainsActiveIntegerAttachments()) {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
            "can't be called on integer buffers");
        return error::kNoError;
      }
    }
    AdjustDrawBuffers();
    api()->glClearFn(mask);
  }
  return error::kNoError;
}
