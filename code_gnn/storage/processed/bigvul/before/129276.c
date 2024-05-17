void GLES2DecoderImpl::DoFramebufferRenderbuffer(
    GLenum target, GLenum attachment, GLenum renderbuffertarget,
    GLuint client_renderbuffer_id) {
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (!framebuffer) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glFramebufferRenderbuffer", "no framebuffer bound");
    return;
  }
  GLuint service_id = 0;
  Renderbuffer* renderbuffer = NULL;
  if (client_renderbuffer_id) {
    renderbuffer = GetRenderbuffer(client_renderbuffer_id);
    if (!renderbuffer) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION,
          "glFramebufferRenderbuffer", "unknown renderbuffer");
      return;
    }
    service_id = renderbuffer->service_id();
  }
  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER("glFramebufferRenderbuffer");
  glFramebufferRenderbufferEXT(
      target, attachment, renderbuffertarget, service_id);
  GLenum error = LOCAL_PEEK_GL_ERROR("glFramebufferRenderbuffer");
  if (error == GL_NO_ERROR) {
    framebuffer->AttachRenderbuffer(attachment, renderbuffer);
  }
  if (framebuffer == framebuffer_state_.bound_draw_framebuffer.get()) {
    framebuffer_state_.clear_state_dirty = true;
  }
  OnFboChanged();
}
