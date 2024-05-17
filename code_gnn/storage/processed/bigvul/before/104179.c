void GLES2DecoderImpl::RestoreCurrentFramebufferBindings() {
  state_dirty_ = true;

  if (!feature_info_->feature_flags().chromium_framebuffer_multisample) {
    RebindCurrentFramebuffer(
        GL_FRAMEBUFFER,
        bound_draw_framebuffer_.get(),
        offscreen_target_frame_buffer_.get());
  } else {
    RebindCurrentFramebuffer(
        GL_READ_FRAMEBUFFER_EXT,
        bound_read_framebuffer_.get(),
        offscreen_target_frame_buffer_.get());
    RebindCurrentFramebuffer(
        GL_DRAW_FRAMEBUFFER_EXT,
        bound_draw_framebuffer_.get(),
        offscreen_target_frame_buffer_.get());
  }
}
