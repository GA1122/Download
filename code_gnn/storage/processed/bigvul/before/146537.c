void WebGLRenderingContextBase::stencilFunc(GLenum func,
                                            GLint ref,
                                            GLuint mask) {
  if (isContextLost())
    return;
  if (!ValidateStencilOrDepthFunc("stencilFunc", func))
    return;
  stencil_func_ref_ = ref;
  stencil_func_ref_back_ = ref;
  stencil_func_mask_ = mask;
  stencil_func_mask_back_ = mask;
  ContextGL()->StencilFunc(func, ref, mask);
}
