void DefaultAudioDestinationHandler::Uninitialize() {
  DCHECK(IsMainThread());

  if (!IsInitialized()) {
    return;
  }

  StopPlatformDestination();
  AudioHandler::Uninitialize();
}
