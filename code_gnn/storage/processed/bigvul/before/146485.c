void WebGLRenderingContextBase::disable(GLenum cap) {
  if (isContextLost() || !ValidateCapability("disable", cap))
    return;
  if (cap == GL_STENCIL_TEST) {
    stencil_enabled_ = false;
    ApplyStencilTest();
    return;
  }
  if (cap == GL_SCISSOR_TEST)
    scissor_enabled_ = false;
  ContextGL()->Disable(cap);
}
