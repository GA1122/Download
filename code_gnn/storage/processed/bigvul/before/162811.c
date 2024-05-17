void ContextState::RestoreAllTextureUnitAndSamplerBindings(
    const ContextState* prev_state) const {
  for (size_t ii = 0; ii < texture_units.size(); ++ii) {
    RestoreTextureUnitBindings(ii, prev_state);
    RestoreSamplerBinding(ii, prev_state);
  }
  RestoreActiveTexture();
}
