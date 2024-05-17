void GLES2DecoderImpl::OnUseFramebuffer() const {
  if (state_.fbo_binding_for_scissor_workaround_dirty_) {
    state_.fbo_binding_for_scissor_workaround_dirty_ = false;
    glScissor(state_.scissor_x,
              state_.scissor_y,
              state_.scissor_width,
              state_.scissor_height);

    glFlush();
  }
}
