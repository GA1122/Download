void AudioHandler::Initialize() {
  CHECK(!g_audio_handler);
  g_audio_handler = new AudioHandler();
}
