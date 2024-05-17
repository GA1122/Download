void OfflineAudioDestinationHandler::Uninitialize() {
  if (!IsInitialized())
    return;

  render_thread_.reset();

  AudioHandler::Uninitialize();
}
