WebGLFramebuffer* WebGL2RenderingContextBase::GetFramebufferBinding(
    GLenum target) {
  switch (target) {
    case GL_READ_FRAMEBUFFER:
      return read_framebuffer_binding_.Get();
    case GL_DRAW_FRAMEBUFFER:
      return framebuffer_binding_.Get();
    default:
      return WebGLRenderingContextBase::GetFramebufferBinding(target);
  }
}
