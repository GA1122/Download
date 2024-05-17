bool GLES2DecoderImpl::CheckBoundDrawFramebufferValid(
    const char* func_name,
    bool check_float_blending) {
  Framebuffer* framebuffer = GetBoundDrawFramebuffer();
  bool valid = CheckFramebufferValid(
      framebuffer, GetDrawFramebufferTarget(),
      GL_INVALID_FRAMEBUFFER_OPERATION, func_name);
  if (!valid)
    return false;

  if (check_float_blending) {
    if (framebuffer && state_.GetEnabled(GL_BLEND) &&
        !features().ext_float_blend) {
      if (framebuffer->HasActiveFloat32ColorAttachment()) {
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
                           "GL_BLEND with floating-point color attachments "
                           "requires the EXT_float_blend extension");
        return false;
      }
    }
  }

  if (!SupportsSeparateFramebufferBinds())
    OnUseFramebuffer();

  UpdateFramebufferSRGB(framebuffer);
  return true;
}
