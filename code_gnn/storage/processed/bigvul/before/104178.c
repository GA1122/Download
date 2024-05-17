void GLES2DecoderImpl::RestoreClearState() {
  state_dirty_ = true;
  glClearColor(clear_red_, clear_green_, clear_blue_, clear_alpha_);
  glClearStencil(clear_stencil_);
  glClearDepth(clear_depth_);
  if (enable_scissor_test_) {
    glEnable(GL_SCISSOR_TEST);
  }
}
