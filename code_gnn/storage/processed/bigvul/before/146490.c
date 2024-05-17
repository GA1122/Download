void WebGLRenderingContextBase::enable(GLenum cap) {
  if (isContextLost() || !ValidateCapability("enable", cap))
    return;
  if (cap == GL_STENCIL_TEST) {
    stencil_enabled_ = true;
    ApplyStencilTest();
    return;
  }
  if (cap == GL_SCISSOR_TEST)
    scissor_enabled_ = true;
  ContextGL()->Enable(cap);
}
