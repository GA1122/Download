void WebGLRenderingContextBase::stencilFuncSeparate(GLenum face,
                                                    GLenum func,
                                                    GLint ref,
                                                    GLuint mask) {
  if (isContextLost())
    return;
  if (!ValidateStencilOrDepthFunc("stencilFuncSeparate", func))
    return;
  switch (face) {
    case GL_FRONT_AND_BACK:
      stencil_func_ref_ = ref;
      stencil_func_ref_back_ = ref;
      stencil_func_mask_ = mask;
      stencil_func_mask_back_ = mask;
      break;
    case GL_FRONT:
      stencil_func_ref_ = ref;
      stencil_func_mask_ = mask;
      break;
    case GL_BACK:
      stencil_func_ref_back_ = ref;
      stencil_func_mask_back_ = mask;
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "stencilFuncSeparate", "invalid face");
      return;
  }
  ContextGL()->StencilFuncSeparate(face, func, ref, mask);
}
