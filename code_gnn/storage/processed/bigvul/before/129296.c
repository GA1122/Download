bool GLES2DecoderImpl::DoIsFramebuffer(GLuint client_id) {
  const Framebuffer* framebuffer =
      GetFramebuffer(client_id);
  return framebuffer && framebuffer->IsValid() && !framebuffer->IsDeleted();
}
