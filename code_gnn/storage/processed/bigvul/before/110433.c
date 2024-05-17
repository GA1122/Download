bool GLES2DecoderImpl::CheckBoundFramebuffersValid(const char* func_name) {
  if (!feature_info_->feature_flags().chromium_framebuffer_multisample) {
    return CheckFramebufferValid(
        bound_draw_framebuffer_, GL_FRAMEBUFFER_EXT, func_name);
  }
  return CheckFramebufferValid(
             bound_draw_framebuffer_, GL_DRAW_FRAMEBUFFER_EXT, func_name) &&
         CheckFramebufferValid(
             bound_read_framebuffer_, GL_READ_FRAMEBUFFER_EXT, func_name);
}
