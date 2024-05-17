static void RebindCurrentFramebuffer(
    GLenum target,
    Framebuffer* framebuffer,
    GLuint back_buffer_service_id) {
  GLuint framebuffer_id = framebuffer ? framebuffer->service_id() : 0;

  if (framebuffer_id == 0) {
    framebuffer_id = back_buffer_service_id;
  }

  glBindFramebufferEXT(target, framebuffer_id);
}