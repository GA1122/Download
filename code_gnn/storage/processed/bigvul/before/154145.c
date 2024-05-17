  Framebuffer* GetBoundReadFramebuffer() const {
    GLenum target = GetReadFramebufferTarget();
    return GetFramebufferInfoForTarget(target);
  }
