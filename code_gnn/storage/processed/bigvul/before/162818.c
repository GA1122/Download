void ContextState::RestoreState(const ContextState* prev_state) {
  RestoreAllTextureUnitAndSamplerBindings(prev_state);
  RestoreVertexAttribs();
  RestoreIndexedUniformBufferBindings(prev_state);
  RestoreBufferBindings();
  RestoreRenderbufferBindings();
  RestoreProgramSettings(prev_state, true);
  RestoreGlobalState(prev_state);

  framebuffer_srgb_valid_ = false;
}
