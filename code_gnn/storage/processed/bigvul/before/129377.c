  Renderbuffer* GetRenderbufferInfoForTarget(
      GLenum target) {
    Renderbuffer* renderbuffer = NULL;
    switch (target) {
      case GL_RENDERBUFFER:
        renderbuffer = state_.bound_renderbuffer.get();
        break;
      default:
        NOTREACHED();
        break;
    }
    return renderbuffer;
  }
