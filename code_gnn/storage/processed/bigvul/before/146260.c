WebGLRenderingContextBase::ClearIfComposited(GLbitfield mask) {
  if (isContextLost())
    return kSkipped;

  if (!GetDrawingBuffer()->BufferClearNeeded() ||
      (mask && framebuffer_binding_))
    return kSkipped;

  Nullable<WebGLContextAttributes> context_attributes;
  getContextAttributes(context_attributes);
  if (context_attributes.IsNull()) {
    return kSkipped;
  }

  bool combined_clear = mask && !scissor_enabled_;

  ContextGL()->Disable(GL_SCISSOR_TEST);
  if (combined_clear && (mask & GL_COLOR_BUFFER_BIT)) {
    ContextGL()->ClearColor(color_mask_[0] ? clear_color_[0] : 0,
                            color_mask_[1] ? clear_color_[1] : 0,
                            color_mask_[2] ? clear_color_[2] : 0,
                            color_mask_[3] ? clear_color_[3] : 0);
  } else {
    ContextGL()->ClearColor(0, 0, 0, 0);
  }
  ContextGL()->ColorMask(
      true, true, true,
      !GetDrawingBuffer()->RequiresAlphaChannelToBePreserved());
  GLbitfield clear_mask = GL_COLOR_BUFFER_BIT;
  if (context_attributes.Get().depth()) {
    if (!combined_clear || !depth_mask_ || !(mask & GL_DEPTH_BUFFER_BIT))
      ContextGL()->ClearDepthf(1.0f);
    clear_mask |= GL_DEPTH_BUFFER_BIT;
    ContextGL()->DepthMask(true);
  }
  if (context_attributes.Get().stencil() ||
      GetDrawingBuffer()->HasImplicitStencilBuffer()) {
    if (combined_clear && (mask & GL_STENCIL_BUFFER_BIT))
      ContextGL()->ClearStencil(clear_stencil_ & stencil_mask_);
    else
      ContextGL()->ClearStencil(0);
    clear_mask |= GL_STENCIL_BUFFER_BIT;
    ContextGL()->StencilMaskSeparate(GL_FRONT, 0xFFFFFFFF);
  }

  ContextGL()->ColorMask(
      true, true, true,
      !GetDrawingBuffer()->DefaultBufferRequiresAlphaChannelToBePreserved());
  GetDrawingBuffer()->ClearFramebuffers(clear_mask);

  DrawingBufferClientRestoreScissorTest();
  DrawingBufferClientRestoreMaskAndClearValues();

  GetDrawingBuffer()->SetBufferClearNeeded(false);

  return combined_clear ? kCombinedClear : kJustClear;
}
