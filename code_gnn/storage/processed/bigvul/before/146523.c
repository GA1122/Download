GLboolean WebGLRenderingContextBase::isEnabled(GLenum cap) {
  if (isContextLost() || !ValidateCapability("isEnabled", cap))
    return 0;
  if (cap == GL_STENCIL_TEST)
    return stencil_enabled_;
  return ContextGL()->IsEnabled(cap);
}
