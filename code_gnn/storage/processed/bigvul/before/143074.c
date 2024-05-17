void DefaultAudioDestinationHandler::StartRendering() {
  DCHECK(IsMainThread());

  StartPlatformDestination();
}
