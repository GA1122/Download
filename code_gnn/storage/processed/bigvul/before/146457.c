GLenum WebGLRenderingContextBase::checkFramebufferStatus(GLenum target) {
  if (isContextLost())
    return GL_FRAMEBUFFER_UNSUPPORTED;
  if (!ValidateFramebufferTarget(target)) {
    SynthesizeGLError(GL_INVALID_ENUM, "checkFramebufferStatus",
                      "invalid target");
    return 0;
  }
  WebGLFramebuffer* framebuffer_binding = GetFramebufferBinding(target);
  if (framebuffer_binding) {
    const char* reason = "framebuffer incomplete";
    GLenum status = framebuffer_binding->CheckDepthStencilStatus(&reason);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      EmitGLWarning("checkFramebufferStatus", reason);
      return status;
    }
  }
  return ContextGL()->CheckFramebufferStatus(target);
}
