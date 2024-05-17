void SystemKeyEventListener::ShowVolumeBubble() {
  AudioHandler* audio_handler = GetAudioHandler();
  if (audio_handler) {
    VolumeBubble::GetInstance()->ShowBubble(
        audio_handler->GetVolumePercent(),
        !audio_handler->IsMuted());
  }
  BrightnessBubble::GetInstance()->HideBubble();
}
