bool GLES2DecoderImpl::CheckBoundFramebufferValid(const char* func_name) {
  GLenum gl_error = GL_INVALID_FRAMEBUFFER_OPERATION;
  Framebuffer* draw_framebuffer = GetBoundDrawFramebuffer();
  bool valid = CheckFramebufferValid(
      draw_framebuffer, GetDrawFramebufferTarget(), gl_error, func_name);

  Framebuffer* read_framebuffer = GetBoundReadFramebuffer();
  valid = valid && CheckFramebufferValid(
      read_framebuffer, GetReadFramebufferTarget(), gl_error, func_name);
  return valid;
}
