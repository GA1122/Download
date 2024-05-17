  GLenum GetReadFramebufferTarget() const {
    return SupportsSeparateFramebufferBinds() ?
        GL_READ_FRAMEBUFFER : GL_FRAMEBUFFER;
  }
