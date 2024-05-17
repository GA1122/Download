void GLES2DecoderImpl::RestoreClearState() {
  framebuffer_state_.clear_state_dirty = true;
  api()->glClearColorFn(state_.color_clear_red, state_.color_clear_green,
                        state_.color_clear_blue, state_.color_clear_alpha);
  api()->glClearStencilFn(state_.stencil_clear);
  api()->glClearDepthFn(state_.depth_clear);
  state_.SetDeviceCapabilityState(GL_SCISSOR_TEST,
                                  state_.enable_flags.scissor_test);
  RestoreDeviceWindowRectangles();
  gfx::Vector2d scissor_offset = GetBoundFramebufferDrawOffset();
  api()->glScissorFn(state_.scissor_x + scissor_offset.x(),
                     state_.scissor_y + scissor_offset.y(),
                     state_.scissor_width, state_.scissor_height);
}
