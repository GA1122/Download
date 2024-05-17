void DefaultAudioDestinationHandler::StopRendering() {
  DCHECK(IsMainThread());

   StopPlatformDestination();
 }
