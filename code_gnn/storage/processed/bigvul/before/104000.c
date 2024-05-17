GLenum GLES2DecoderImpl::DoCheckFramebufferStatus(GLenum target) {
  FramebufferManager::FramebufferInfo* info =
      GetFramebufferInfoForTarget(target);
  if (!info) {
    return GL_FRAMEBUFFER_COMPLETE;
  }
  return glCheckFramebufferStatusEXT(target);
}
