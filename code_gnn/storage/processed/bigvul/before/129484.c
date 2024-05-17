void GLES2DecoderImpl::RestoreClearState() {
  framebuffer_state_.clear_state_dirty = true;
  glClearColor(
      state_.color_clear_red, state_.color_clear_green, state_.color_clear_blue,
      state_.color_clear_alpha);
  glClearStencil(state_.stencil_clear);
  glClearDepth(state_.depth_clear);
  if (state_.enable_flags.scissor_test) {
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, true);
  }
}
