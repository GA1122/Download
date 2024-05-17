void WebGL2RenderingContextBase::bindFramebuffer(GLenum target,
                                                 WebGLFramebuffer* buffer) {
  bool deleted;
  if (!CheckObjectToBeBound("bindFramebuffer", buffer, deleted))
    return;

  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindFramebuffer",
                      "attempt to bind a deleted framebuffer");
    return;
  }

  switch (target) {
    case GL_DRAW_FRAMEBUFFER:
      break;
    case GL_FRAMEBUFFER:
    case GL_READ_FRAMEBUFFER:
      read_framebuffer_binding_ = buffer;
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "bindFramebuffer", "invalid target");
      return;
  }

  SetFramebuffer(target, buffer);
}
