void WebGLRenderingContextBase::clear(GLbitfield mask) {
  if (isContextLost())
    return;
  if (mask &
      ~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)) {
    SynthesizeGLError(GL_INVALID_VALUE, "clear", "invalid mask");
    return;
  }
  const char* reason = "framebuffer incomplete";
  if (framebuffer_binding_ && framebuffer_binding_->CheckDepthStencilStatus(
                                  &reason) != GL_FRAMEBUFFER_COMPLETE) {
    SynthesizeGLError(GL_INVALID_FRAMEBUFFER_OPERATION, "clear", reason);
    return;
  }

  ScopedRGBEmulationColorMask emulation_color_mask(this, color_mask_,
                                                   drawing_buffer_.get());

  if (ClearIfComposited(mask) != kCombinedClear) {
    if (!framebuffer_binding_ &&
        GetDrawingBuffer()->HasImplicitStencilBuffer() &&
        (mask & GL_DEPTH_BUFFER_BIT)) {
      mask |= GL_STENCIL_BUFFER_BIT;
    }
    ContextGL()->Clear(mask);
  }
  MarkContextChanged(kCanvasChanged);
}
