bool GLES2DecoderImpl::CheckBoundReadFramebufferValid(
    const char* func_name, GLenum gl_error) {
  Framebuffer* framebuffer = GetBoundReadFramebuffer();
  bool valid = CheckFramebufferValid(
      framebuffer, GetReadFramebufferTarget(), gl_error, func_name);
  return valid;
}
