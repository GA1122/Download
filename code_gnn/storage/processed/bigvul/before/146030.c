bool WebGL2RenderingContextBase::ValidateFramebufferTarget(GLenum target) {
  switch (target) {
    case GL_FRAMEBUFFER:
    case GL_READ_FRAMEBUFFER:
    case GL_DRAW_FRAMEBUFFER:
      return true;
    default:
      return false;
  }
}
