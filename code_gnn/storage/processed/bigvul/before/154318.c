void GLES2DecoderImpl::OnFboChanged() const {
  state_.fbo_binding_for_scissor_workaround_dirty = true;
  state_.stencil_state_changed_since_validation = true;

  if (workarounds().flush_on_framebuffer_change)
    api()->glFlushFn();
}
