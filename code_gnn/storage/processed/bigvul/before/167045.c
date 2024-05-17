void MetalayerMode::OnVoiceInteractionStatusChanged(
    mojom::VoiceInteractionState state) {
  voice_interaction_state_ = state;
  UpdateState();
}
