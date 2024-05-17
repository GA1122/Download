void AudioHandler::Shutdown() {
  if (g_audio_handler) {
    delete g_audio_handler;
    g_audio_handler = NULL;
  }
}
