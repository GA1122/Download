void OfflineAudioDestinationHandler::Initialize() {
  if (IsInitialized())
    return;

  AudioHandler::Initialize();
}
