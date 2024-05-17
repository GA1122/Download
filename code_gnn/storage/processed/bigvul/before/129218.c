bool GLES2DecoderImpl::CheckBoundFramebuffersValid(const char* func_name) {
  if (!features().chromium_framebuffer_multisample) {
    bool valid = CheckFramebufferValid(
        framebuffer_state_.bound_draw_framebuffer.get(), GL_FRAMEBUFFER_EXT,
        func_name);

    if (valid)
      OnUseFramebuffer();

    return valid;
  }
  return CheckFramebufferValid(framebuffer_state_.bound_draw_framebuffer.get(),
                               GL_DRAW_FRAMEBUFFER_EXT,
                               func_name) &&
         CheckFramebufferValid(framebuffer_state_.bound_read_framebuffer.get(),
                               GL_READ_FRAMEBUFFER_EXT,
                               func_name);
}
