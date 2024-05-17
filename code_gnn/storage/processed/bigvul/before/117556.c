AudioHandler* SystemKeyEventListener::GetAudioHandler() const {
  AudioHandler* audio_handler = AudioHandler::GetInstance();
  if (!audio_handler || !audio_handler->IsInitialized())
    return NULL;
  return audio_handler;
}
