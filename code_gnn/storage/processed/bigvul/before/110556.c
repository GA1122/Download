  RenderbufferManager::RenderbufferInfo* GetRenderbufferInfoForTarget(
      GLenum target) {
    RenderbufferManager::RenderbufferInfo* info = NULL;
    switch (target) {
      case GL_RENDERBUFFER:
        info = bound_renderbuffer_;
        break;
      default:
        NOTREACHED();
        break;
    }
    return info;
  }
