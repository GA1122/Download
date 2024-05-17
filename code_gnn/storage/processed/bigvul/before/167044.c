void MetalayerMode::OnVoiceInteractionSettingsEnabled(bool enabled) {
  voice_interaction_enabled_ = enabled;
  UpdateState();
}
