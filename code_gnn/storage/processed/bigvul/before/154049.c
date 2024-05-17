void GLES2DecoderImpl::DoReadBuffer(GLenum src) {
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(GL_READ_FRAMEBUFFER);
  if (framebuffer) {
    if (src == GL_BACK) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_ENUM, "glReadBuffer",
          "invalid src for a named framebuffer");
      return;
    }
    framebuffer->set_read_buffer(src);
  } else {
    if (src != GL_NONE && src != GL_BACK) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_ENUM, "glReadBuffer",
          "invalid src for the default framebuffer");
      return;
    }
    back_buffer_read_buffer_ = src;
    if (GetBackbufferServiceId() && src == GL_BACK)
      src = GL_COLOR_ATTACHMENT0;
  }
  api()->glReadBufferFn(src);
}
