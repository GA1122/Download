GLenum GLES2DecoderImpl::DoCheckFramebufferStatus(GLenum target) {
  FramebufferManager::FramebufferInfo* framebuffer =
      GetFramebufferInfoForTarget(target);
  if (!framebuffer) {
    return GL_FRAMEBUFFER_COMPLETE;
  }
  GLenum completeness = framebuffer->IsPossiblyComplete();
  if (completeness != GL_FRAMEBUFFER_COMPLETE) {
    return completeness;
  }
  return glCheckFramebufferStatusEXT(target);
}
