bool GLES2DecoderImpl::CheckBoundDrawFramebufferValid(const char* func_name) {
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  bool valid = CheckFramebufferValid(
      framebuffer, GetDrawFramebufferTarget(),
      GL_INVALID_FRAMEBUFFER_OPERATION, func_name);
  if (!valid)
    return false;

  if (!SupportsSeparateFramebufferBinds())
    OnUseFramebuffer();

  UpdateFramebufferSRGB(framebuffer);
  return true;
}
