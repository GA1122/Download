void GLES2DecoderImpl::OnFboChanged() const {
  if (workarounds().restore_scissor_on_fbo_change)
    state_.fbo_binding_for_scissor_workaround_dirty_ = true;
}
