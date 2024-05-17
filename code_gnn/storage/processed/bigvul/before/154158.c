  GLenum GetDrawFramebufferTarget() const {
    return SupportsSeparateFramebufferBinds() ?
        GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER;
  }
