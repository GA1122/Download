void OfflineAudioDestinationHandler::StartOfflineRendering() {
  DCHECK(!IsMainThread());

  DCHECK(render_bus_);
  if (!render_bus_)
    return;

  bool is_audio_context_initialized = Context()->IsDestinationInitialized();
  DCHECK(is_audio_context_initialized);
  if (!is_audio_context_initialized)
    return;

  bool channels_match =
      render_bus_->NumberOfChannels() == render_target_->numberOfChannels();
  DCHECK(channels_match);
  if (!channels_match)
    return;

  bool is_render_bus_allocated =
      render_bus_->length() >= audio_utilities::kRenderQuantumFrames;
  DCHECK(is_render_bus_allocated);
  if (!is_render_bus_allocated)
    return;

  DoOfflineRendering();
}
