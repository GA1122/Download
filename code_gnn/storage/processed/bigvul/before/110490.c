bool GLES2DecoderImpl::DoIsFramebuffer(GLuint client_id) {
  const FramebufferManager::FramebufferInfo* framebuffer =
      GetFramebufferInfo(client_id);
  return framebuffer && framebuffer->IsValid() && !framebuffer->IsDeleted();
}
