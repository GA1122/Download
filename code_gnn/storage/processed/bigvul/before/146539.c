void WebGLRenderingContextBase::stencilMask(GLuint mask) {
  if (isContextLost())
    return;
  stencil_mask_ = mask;
  stencil_mask_back_ = mask;
  ContextGL()->StencilMask(mask);
}
