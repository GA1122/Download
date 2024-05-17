void WebGLRenderingContextBase::bindFramebuffer(GLenum target,
                                                WebGLFramebuffer* buffer) {
  bool deleted;
  if (!CheckObjectToBeBound("bindFramebuffer", buffer, deleted))
    return;
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindFramebuffer",
                      "attempt to bind a deleted framebuffer");
    return;
  }

  if (target != GL_FRAMEBUFFER) {
    SynthesizeGLError(GL_INVALID_ENUM, "bindFramebuffer", "invalid target");
    return;
  }

  SetFramebuffer(target, buffer);
}
