void ContextState::RestoreIndexedUniformBufferBindings(
    const ContextState* prev_state) {
  if (!feature_info_->IsES3Capable())
    return;
  indexed_uniform_buffer_bindings->RestoreBindings(
      prev_state ? prev_state->indexed_uniform_buffer_bindings.get() : nullptr);
}
