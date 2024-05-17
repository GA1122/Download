void GLES2DecoderImpl::RestoreCurrentFramebufferBindings() {
  framebuffer_state_.clear_state_dirty = true;

  if (!SupportsSeparateFramebufferBinds()) {
    RebindCurrentFramebuffer(api(), GL_FRAMEBUFFER,
                             framebuffer_state_.bound_draw_framebuffer.get(),
                             GetBackbufferServiceId());
  } else {
    RebindCurrentFramebuffer(api(), GL_READ_FRAMEBUFFER_EXT,
                             framebuffer_state_.bound_read_framebuffer.get(),
                             GetBackbufferServiceId());
    RebindCurrentFramebuffer(api(), GL_DRAW_FRAMEBUFFER_EXT,
                             framebuffer_state_.bound_draw_framebuffer.get(),
                             GetBackbufferServiceId());
  }
  OnFboChanged();
}
