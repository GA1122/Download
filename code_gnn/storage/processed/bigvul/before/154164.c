  Framebuffer* GetFramebufferInfoForTarget(GLenum target) const {
    Framebuffer* framebuffer = nullptr;
    switch (target) {
      case GL_FRAMEBUFFER:
      case GL_DRAW_FRAMEBUFFER_EXT:
        framebuffer = framebuffer_state_.bound_draw_framebuffer.get();
        break;
      case GL_READ_FRAMEBUFFER_EXT:
        framebuffer = framebuffer_state_.bound_read_framebuffer.get();
        break;
      default:
        NOTREACHED();
        break;
    }
    return framebuffer;
  }
