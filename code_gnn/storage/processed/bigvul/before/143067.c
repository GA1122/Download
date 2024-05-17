void DefaultAudioDestinationHandler::Initialize() {
  DCHECK(IsMainThread());

  CreatePlatformDestination();
  AudioHandler::Initialize();
}
