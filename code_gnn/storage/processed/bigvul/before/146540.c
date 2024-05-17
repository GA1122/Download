void WebGLRenderingContextBase::stencilMaskSeparate(GLenum face, GLuint mask) {
  if (isContextLost())
    return;
  switch (face) {
    case GL_FRONT_AND_BACK:
      stencil_mask_ = mask;
      stencil_mask_back_ = mask;
      break;
    case GL_FRONT:
      stencil_mask_ = mask;
      break;
    case GL_BACK:
      stencil_mask_back_ = mask;
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "stencilMaskSeparate", "invalid face");
      return;
  }
  ContextGL()->StencilMaskSeparate(face, mask);
}
